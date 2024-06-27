#include "Simulation.h"
#include "core/gl/color.h"
#include <math/quaternion.h>

#include <memory>
#include <utility>

namespace yage::physics3d
{
    Simulation::Simulation(Visualizer visualizer)
    {
        m_visualizer = std::make_unique<Visualizer>(std::move(visualizer));
    }

    math::Matd<12, 12> Simulation::inverse_mass_matrix(const RigidBody& a, const RigidBody& b)
    {
        // TODO: instead of this sparse matrix multiplication, use the direct formula for the effective mass matrix
        math::Matd<12, 12> m_inv;
        m_inv(0, 0) = a.m_inertia_shape.inverse_mass();
        m_inv(1, 1) = a.m_inertia_shape.inverse_mass();
        m_inv(2, 2) = a.m_inertia_shape.inverse_mass();
        m_inv(3, 3) = b.m_inertia_shape.inverse_mass();
        m_inv(4, 4) = b.m_inertia_shape.inverse_mass();
        m_inv(5, 5) = b.m_inertia_shape.inverse_mass();

        auto inv_inertia_a = a.m_inertia_shape.inverse_inertia_tensor();
        m_inv(6, 6) = inv_inertia_a(0, 0);
        m_inv(6, 7) = inv_inertia_a(0, 1);
        m_inv(6, 8) = inv_inertia_a(0, 2);
        m_inv(7, 6) = inv_inertia_a(1, 0);
        m_inv(7, 7) = inv_inertia_a(1, 1);
        m_inv(7, 8) = inv_inertia_a(1, 2);
        m_inv(8, 6) = inv_inertia_a(2, 0);
        m_inv(8, 7) = inv_inertia_a(2, 1);
        m_inv(8, 8) = inv_inertia_a(2, 2);

        auto inv_inertia_b = b.m_inertia_shape.inverse_inertia_tensor();
        m_inv(9, 9) = inv_inertia_b(0, 0);
        m_inv(9, 10) = inv_inertia_b(0, 1);
        m_inv(9, 11) = inv_inertia_b(0, 2);
        m_inv(10, 9) = inv_inertia_b(1, 0);
        m_inv(10, 10) = inv_inertia_b(1, 1);
        m_inv(10, 11) = inv_inertia_b(1, 2);
        m_inv(11, 9) = inv_inertia_b(2, 0);
        m_inv(11, 10) = inv_inertia_b(2, 1);
        m_inv(11, 11) = inv_inertia_b(2, 2);

        return m_inv;
    }

    std::tuple<math::Vec3d, math::Vec3d> Simulation::tangent_plane(const math::Vec3d& n)
    {
        // TODO: this can be optimized if we simplify the formulas and assume |n| = 1

        // choose linearly independent (non-parallel) vector to n by using the basis-vector in the direction of the smallest element
        math::Vec3d u1{};
        // TODO: build an argmax function
        if (std::abs(n.x()) < std::abs(n.y())) {
            if (std::abs(n.z()) < std::abs(n.x())) {
                u1.z() = 1;
            } else {
                u1.x() = 1;
            }
        } else {
            if (std::abs(n.z()) < std::abs(n.y())) {
                u1.z() = 1;
            } else {
                u1.y() = 1;
            }
        }

        // Gram-Schmidt method: project u1 onto n and subtract from u1 to get orthogonal vector
        u1 -= math::dot(u1, n) * n; // denominator not needed, as n is unit length
        u1.normalize();

        // find second orthogonal vector; since n and u1 are orthonormal, u2 is already normalized
        auto u2 = math::cross(u1, n);
        return {u1, u2};
    }

    double Simulation::solve_constraint(const Constraint& constraint)
    {
        auto& a = constraint.rb_a;
        auto& b = constraint.rb_b;

        const math::Matd<12, 1> q_pre{
                a.m_velocity.x(), a.m_velocity.y(), a.m_velocity.z(),
                b.m_velocity.x(), b.m_velocity.y(), b.m_velocity.z(),
                a.m_angular_velocity.x(), a.m_angular_velocity.y(), a.m_angular_velocity.z(),
                b.m_angular_velocity.x(), b.m_angular_velocity.y(), b.m_angular_velocity.z(),
        };

        math::Matd<1, 1> nominator = -(constraint.j * q_pre + math::Matd<1, 1>(constraint.bias));
        math::Matd<1, 1> denominator = constraint.j * constraint.m_inv * constraint.j_t;
        const double delta_lambda = nominator(0, 0) / denominator(0, 0);

        return delta_lambda;
    }

    void Simulation::update(const double dt)
    {
        remove_destroyed_bodies();
        integrate_forces(dt);
        detect_collisions(dt);
        resolve_collisions(dt);
        integrate_positions(dt);
        clear_forces();
    }

    void Simulation::update_staggered(const double dt)
    {
        remove_destroyed_bodies();
        resolve_collisions(dt);
        integrate_positions(dt);

        integrate_forces(dt);
        detect_collisions(dt);
        clear_forces();
    }

    RigidBody& Simulation::lookup(const RigidBodyHandle handle)
    {
        return m_bodies[handle.id];
    }

    void Simulation::enable_gravity()
    {
        m_external_acceleration = {0, -9.81, 0};
    }

    void Simulation::disable_gravity()
    {
        m_external_acceleration = {0, 0, 0};
    }

    Constraint
    Simulation::prepare_penetration_constraint(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold,
                                               const ContactPoint& contact, const double dt) const
    {
        math::Vec3d temp_1 = cross(contact.r_a, manifold.normal);
        math::Vec3d temp_2 = cross(contact.r_b, manifold.normal);
        const math::Matd<1, 12> j{
                -manifold.normal.x(), -manifold.normal.y(), -manifold.normal.z(),
                manifold.normal.x(), manifold.normal.y(), manifold.normal.z(),
                -temp_1.x(), -temp_1.y(), -temp_1.z(),
                temp_2.x(), temp_2.y(), temp_2.z(),
        };

        const double baumgarte_bias = -m_baumgarte_factor / dt * std::max(contact.depth - m_penetration_slop, 0.0);

        // TODO: maybe we can scale the restitution slop with something
        const double restitution = std::min(rb_a.material.restitution, rb_b.material.restitution);
        const double restitution_bias = restitution * std::min(contact.rel_v_n + m_restitution_slop, 0.0);

        return {
                .m_inv = inverse_mass_matrix(rb_a, rb_b),
                .j = j,
                .j_t = transpose(j),
                // don't add the biases, since the baumgarte bias is already satisfied if there's enough restitution
                .bias = std::min(baumgarte_bias, restitution_bias),
                .rb_a = rb_a,
                .rb_b = rb_b,
        };
    }

    std::tuple<Constraint, Constraint>
    Simulation::prepare_friction_constraints(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold,
                                             const ContactPoint& contact)
    {
        // friction along first tangent
        math::Vec3d temp_1 = cross(contact.r_a, manifold.tangent_1);
        math::Vec3d temp_2 = cross(contact.r_b, manifold.tangent_1);
        const math::Matd<1, 12> j_1{
                -manifold.tangent_1.x(), -manifold.tangent_1.y(), -manifold.tangent_1.z(),
                manifold.tangent_1.x(), manifold.tangent_1.y(), manifold.tangent_1.z(),
                -temp_1.x(), -temp_1.y(), -temp_1.z(),
                temp_2.x(), temp_2.y(), temp_2.z(),
        };

        // friction along second tangent
        temp_1 = cross(contact.r_a, manifold.tangent_2);
        temp_2 = cross(contact.r_b, manifold.tangent_2);
        const math::Matd<1, 12> j_2{
                -manifold.tangent_2.x(), -manifold.tangent_2.y(), -manifold.tangent_2.z(),
                manifold.tangent_2.x(), manifold.tangent_2.y(), manifold.tangent_2.z(),
                -temp_1.x(), -temp_1.y(), -temp_1.z(),
                temp_2.x(), temp_2.y(), temp_2.z(),
        };

        const auto m_inv = inverse_mass_matrix(rb_a, rb_b);
        return {
                {
                        .m_inv = m_inv,
                        .j = j_1,
                        .j_t = transpose(j_1),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                },
                {
                        .m_inv = m_inv,
                        .j = j_2,
                        .j_t = transpose(j_2),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                }
        };
    }

    std::tuple<Constraint, Constraint, Constraint>
    Simulation::prepare_rolling_friction_constraints(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold)
    {
        const math::Matd<1, 12> j_0{
                0, 0, 0,
                0, 0, 0,
                -manifold.normal.x(), -manifold.normal.y(), -manifold.normal.z(),
                manifold.normal.x(), manifold.normal.y(), manifold.normal.z(),
        };

        const math::Matd<1, 12> j_1{
                0, 0, 0,
                0, 0, 0,
                -manifold.tangent_1.x(), -manifold.tangent_1.y(), -manifold.tangent_1.z(),
                manifold.tangent_1.x(), manifold.tangent_1.y(), manifold.tangent_1.z(),
        };

        const math::Matd<1, 12> j_2{
                0, 0, 0,
                0, 0, 0,
                -manifold.tangent_2.x(), -manifold.tangent_2.y(), -manifold.tangent_2.z(),
                manifold.tangent_2.x(), manifold.tangent_2.y(), manifold.tangent_2.z(),
        };

        const auto m_inv = inverse_mass_matrix(rb_a, rb_b);
        return {
                {
                        .m_inv = m_inv,
                        .j = j_0,
                        .j_t = transpose(j_0),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                },
                {
                        .m_inv = m_inv,
                        .j = j_1,
                        .j_t = transpose(j_1),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                },
                {
                        .m_inv = m_inv,
                        .j = j_2,
                        .j_t = transpose(j_2),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                }
        };
    }

    void Simulation::apply_impulse(const Constraint& constraint, const math::Matd<12, 1>& impulse)
    {
        auto delta_q = constraint.m_inv * impulse;
        constraint.rb_a.m_velocity += {delta_q(0, 0), delta_q(1, 0), delta_q(2, 0)};
        constraint.rb_b.m_velocity += {delta_q(3, 0), delta_q(4, 0), delta_q(5, 0)};
        constraint.rb_a.m_angular_velocity += {delta_q(6, 0), delta_q(7, 0), delta_q(8, 0)};
        constraint.rb_b.m_angular_velocity += {delta_q(9, 0), delta_q(10, 0), delta_q(11, 0)};
    }

    void Simulation::resolve_penetration_constraint(Constraint& constraint)
    {
        // accumulate impulses
        const double old_lambda = constraint.accumulated_lambda;
        double delta_lambda = solve_constraint(constraint);
        // clamp to prevent objects pulling together for negative lambdas
        constraint.accumulated_lambda = std::max(0.0, constraint.accumulated_lambda + delta_lambda);
        // restore delta lambda after clamping
        delta_lambda = constraint.accumulated_lambda - old_lambda;
        const auto delta_impulse = delta_lambda * constraint.j_t;
        apply_impulse(constraint, delta_impulse);
    }

    void Simulation::resolve_friction_constraint(Constraint& constraint)
    {
        // accumulate impulses
        const double old_lambda = constraint.accumulated_lambda;
        double delta_lambda = solve_constraint(constraint);
        // clamp with accumulated normal impulse for the Coulomb friction model
        const double friction_coefficient =
                constraint.rb_a.material.kinetic_friction *
                constraint.rb_b.material.kinetic_friction;
        const double lambda_n = constraint.dependent_constraint.value()->accumulated_lambda;
        constraint.accumulated_lambda = math::clamp(
                constraint.accumulated_lambda + delta_lambda,
                -friction_coefficient * lambda_n,
                friction_coefficient * lambda_n);
        // restore delta lambda after clamping
        delta_lambda = constraint.accumulated_lambda - old_lambda;
        const auto delta_impulse = delta_lambda * constraint.j_t;
        apply_impulse(constraint, delta_impulse);
    }

    void Simulation::resolve_rolling_friction_constraint(Constraint& constraint)
    {
        solve_constraint(constraint);
        // accumulate impulses TODO: is clamping necessary?
        const double delta_lambda = solve_constraint(constraint);
        constraint.accumulated_lambda += delta_lambda;

        const double friction_coefficient =
                constraint.rb_a.material.rolling_friction *
                constraint.rb_b.material.rolling_friction;
        // TODO: can we do something more physically accurate?
        const auto delta_impulse = friction_coefficient * delta_lambda * constraint.j_t;
        apply_impulse(constraint, delta_impulse);
    }

    void Simulation::visualize_collisions(const math::Mat4d& projection, const math::Mat4d& view) const
    {
        if (m_visualizer) {
            m_visualizer->draw(projection, view);
        }
    }

    void Simulation::integrate_forces(const double dt)
    {
        for (RigidBody& rb : m_bodies) {
            if (rb.should_ignore()) {
                continue;
            }

            // external forces
            if (rb.m_inertia_shape.inverse_mass() > 0) {
                rb.m_velocity += m_external_acceleration * dt;
            }

            // linear component
            rb.m_velocity += rb.m_inertia_shape.inverse_mass() * rb.m_force * dt;

            // angular component
            rb.m_angular_velocity += rb.m_inertia_shape.inverse_inertia_tensor() * rb.m_torque * dt;
        }
    }

    void Simulation::integrate_positions(const double dt)
    {
        for (RigidBody& rb: m_bodies) {
            if (rb.should_ignore()) {
                continue;
            }

            // linear component
            rb.m_position += rb.m_velocity * dt;

            // angular component
            rb.m_orientation += 0.5 * math::Quatd(rb.m_angular_velocity) * rb.m_orientation * dt;
            rb.m_orientation.normalize();

            rb.update_collider();
        }
    }

    void Simulation::detect_collisions(const double dt)
    {
        m_penetration_constraints.clear();
        m_friction_constraints.clear();
        m_rolling_friction_constraints.clear();

        if (m_visualizer) {
            m_visualizer->points.clear();
            m_visualizer->vectors.clear();
        }

        // collision detection narrow phase
        if (m_bodies.size() < 2) {
            return;
        }
        // bodies must be at least of size 2 for this to work (unsigned index)
        for (std::size_t i = 0; i < m_bodies.size() - 1; ++i) {
            RigidBody& rb_a = m_bodies[i];
            if (rb_a.should_ignore() || !rb_a.m_collider.has_value()) {
                continue;
            }

            for (std::size_t j = i + 1; j < m_bodies.size(); ++j) {
                RigidBody& rb_b = m_bodies[j];
                if (rb_b.should_ignore() || !rb_b.m_collider.has_value()) {
                    continue;
                }

                std::optional<ContactManifold> result = std::visit(m_collision_visitor,
                                                                   rb_a.m_collider.value(),
                                                                   rb_b.m_collider.value());
                if (result.has_value()) {
                    ContactManifold& manifold = result.value();
                    for (ContactPoint& contact: manifold.contacts) {
                        math::Vec3d v_abs_p_a = rb_a.m_velocity + cross(rb_a.m_angular_velocity, contact.r_a);
                        math::Vec3d v_abs_p_b = rb_b.m_velocity + cross(rb_b.m_angular_velocity, contact.r_b);
                        contact.rel_v = v_abs_p_b - v_abs_p_a;
                        contact.rel_v_n = dot(contact.rel_v, manifold.normal);

                        // Gram-Schmidt method using the relative velocity as the initial vector for the projection
                        // don't normalize tangent here, since it might be zero-length
                        manifold.tangent_1 = contact.rel_v - manifold.normal * dot(contact.rel_v, manifold.normal);
                        if (length_sqr(manifold.tangent_1) < 0.0000001) {
                            // tangent is parallel to n, so we need another approach
                            std::tie(manifold.tangent_1, manifold.tangent_2) = tangent_plane(manifold.normal);
                        } else {
                            manifold.tangent_1.normalize();
                            // normalization not necessary, since tangent and normal are already normalized
                            manifold.tangent_2 = cross(manifold.tangent_1, manifold.normal);
                        }

                        m_penetration_constraints.push_back(
                                prepare_penetration_constraint(rb_a, rb_b, manifold, contact, dt));

                        auto [cf_0, cf_1] = prepare_friction_constraints(rb_a, rb_b, manifold, contact);
                        m_friction_constraints.push_back(cf_0);
                        m_friction_constraints.push_back(cf_1);

                        auto [crf_0, crf_1, crf_2] = prepare_rolling_friction_constraints(rb_a, rb_b, manifold);
                        m_rolling_friction_constraints.push_back(crf_0);
                        m_rolling_friction_constraints.push_back(crf_1);
                        m_rolling_friction_constraints.push_back(crf_2);

                        if (m_visualizer) {
                            m_visualizer->points.emplace_back(contact.p_a, gl::Color::GREEN);
                            m_visualizer->points.emplace_back(contact.p_b, gl::Color::BLUE);
                        }
                    }
                }
            }
        }

        // once all constraints are created, we can construct the dependencies
        for (std::size_t i = 0; i < m_friction_constraints.size(); ++i) {
            m_friction_constraints.at(i).dependent_constraint = {&m_penetration_constraints.at(i / 2)};
        }
    }

    void Simulation::resolve_collisions(double)
    {
        for (int i = 0; i < m_solver_iterations; ++i) {
            // don't interleave constraints, since the friction impulse depends on the normal impulse
            for (Constraint& constraint: m_penetration_constraints) {
                resolve_penetration_constraint(constraint);
            }
            for (Constraint& constraint: m_friction_constraints) {
                resolve_friction_constraint(constraint);
            }
            for (Constraint& constraint: m_rolling_friction_constraints) {
                resolve_rolling_friction_constraint(constraint);
            }
        }
    }

    void Simulation::remove_destroyed_bodies()
    {
        for (std::size_t i = 0; i < m_bodies.size(); ++i) {
            if (m_bodies[i].m_destruction_pending) {
                m_free_ids.push(i);
                m_bodies[i].m_destruction_pending = false;
                m_bodies[i].m_destroyed = true;
            }
        }
    }

    void Simulation::clear_forces()
    {
        for (RigidBody& rb : m_bodies) {
            rb.m_force = math::Vec3d();
            rb.m_torque = math::Vec3d();
        }
    }
}

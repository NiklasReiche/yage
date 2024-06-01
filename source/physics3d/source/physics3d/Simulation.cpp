#include "Simulation.h"
#include <gml/quaternion.h>
#include <functional>

namespace physics3d
{
    gml::Matd<12, 12> Simulation::inverse_mass_matrix(RigidBody& a, RigidBody& b)
    {
        // TODO: instead of this sparse matrix multiplication, use the direct formula for the effective mass matrix
        gml::Matd<12, 12> m_inv;
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

    double Simulation::solve(gml::Matd<12, 12> m_inv, gml::Matd<1, 12> j, gml::Matd<12, 1> j_t, gml::Matd<12, 1> q_pre,
                             double b)
    {
        auto nominator = -(j * q_pre + gml::Matd<1, 1>(b));
        auto denominator = j * m_inv * j_t;
        return nominator(0, 0) / denominator(0, 0);
    }

    std::tuple<gml::Vec3d, gml::Vec3d> Simulation::tangent_plane(const gml::Vec3d& n)
    {
        // TODO: this can be optimized if we simplify the formulas and assume |n| = 1

        // choose linearly independent (non-parallel) vector to n by using the basis-vector in the direction of the smallest element
        gml::Vec3d u1{};
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
        u1 -= gml::dot(u1, n) * n; // denominator not needed, as n is unit length
        u1.normalize();

        // find second orthogonal vector; since n and u1 are orthonormal, u2 is already normalized
        auto u2 = gml::cross(u1, n);
        return {u1, u2};
    }


    double Simulation::solve_constraint(Constraint& constraint)
    {
        auto& a = constraint.rb_a;
        auto& b = constraint.rb_b;

        gml::Matd<12, 1> q_pre{
                a.m_velocity.x(), a.m_velocity.y(), a.m_velocity.z(),
                b.m_velocity.x(), b.m_velocity.y(), b.m_velocity.z(),
                a.m_angular_velocity.x(), a.m_angular_velocity.y(), a.m_angular_velocity.z(),
                b.m_angular_velocity.x(), b.m_angular_velocity.y(), b.m_angular_velocity.z(),
        };

        gml::Matd<1, 1> nominator = -(constraint.j * q_pre + gml::Matd<1, 1>(constraint.bias));
        gml::Matd<1, 1> denominator = constraint.j * constraint.m_inv * constraint.j_t;
        double delta_lambda = nominator(0, 0) / denominator(0, 0);

        return delta_lambda;
    }

#if 0
    void Simulation::resolve_collision(Collision& collision, double dt)
    {
        auto& a = collision.rb_a;
        auto& b = collision.rb_b;

        auto m_inv = inverse_mass_matrix(a, b);

        gml::Matd<12, 1> q_pre{
                a.m_velocity.x(), a.m_velocity.y(), a.m_velocity.z(),
                b.m_velocity.x(), b.m_velocity.y(), b.m_velocity.z(),
                a.m_angular_velocity.x(), a.m_angular_velocity.y(), a.m_angular_velocity.z(),
                b.m_angular_velocity.x(), b.m_angular_velocity.y(), b.m_angular_velocity.z(),
        };

        auto& r_a = collision.contact_manifold.contacts.r_a;
        auto& r_b = collision.contact_manifold.contacts.r_b;
        auto& n = collision.contact_manifold.contacts.normal;
        auto depth = collision.depth;
        auto v_rel = collision.rel_v;

        // Gram-Schmidt method using the relative velocity as the initial vector for the projection
        gml::Vec3d u1 = v_rel - n * gml::dot(v_rel, n); // non-normalized, since it might be zero-length
        gml::Vec3d u2;
        if (gml::sqrLength(u1) < 0.0000001) { // u1 is parallel to n, so we need another approach
            std::tie(u1, u2) = tangent_plane(n);
        } else {
            u1.normalize();
            u2 = gml::cross(u1, n); // normalization not necessary, since u1 and n are already normalized
        }

        // penetration
        auto t_1 = gml::cross(r_a, n);
        auto t_2 = gml::cross(r_b, n);
        gml::Matd<1, 12> j_n{
                -n.x(), -n.y(), -n.z(),
                n.x(), n.y(), n.z(),
                -t_1.x(), -t_1.y(), -t_1.z(),
                t_2.x(), t_2.y(), t_2.z(),
        };
        auto j_n_t = gml::transpose(j_n);
        auto baumgarte_bias = -m_baumgarte_factor / dt * std::max(depth - m_penetration_slop, 0.0);
        auto v_rel_n = gml::dot(v_rel, n);
        // TODO: maybe we can scale the slop with something
        const double restitution = std::min(a.material.restitution, b.material.restitution);
        auto restitution_bias = restitution * std::min(v_rel_n + m_restitution_slop, 0.0);
        auto bias = baumgarte_bias + restitution_bias;

        // accumulate impulses
        double old_lambda_n = collision.contact_manifold.lambda_n;
        double lambda_n = solve(m_inv, j_n, j_n_t, q_pre, bias);
        collision.contact_manifold.lambda_n += lambda_n;
        // clamp to prevent objects pulling together for negative lambdas
        collision.contact_manifold.lambda_n = std::max(0.0, collision.contact_manifold.lambda_n);
        // restore delta lambda after clamping
        lambda_n = collision.contact_manifold.lambda_n - old_lambda_n;
        auto p_c = lambda_n * j_n_t;

        auto q_post = q_pre + m_inv * p_c;
        a.m_velocity = {q_post(0, 0), q_post(1, 0), q_post(2, 0)};
        b.m_velocity = {q_post(3, 0), q_post(4, 0), q_post(5, 0)};
        a.m_angular_velocity = {q_post(6, 0), q_post(7, 0), q_post(8, 0)};
        b.m_angular_velocity = {q_post(9, 0), q_post(10, 0), q_post(11, 0)};
    }

    void Simulation::resolve_collision_f(Collision& collision)
    {
        auto& a = collision.rb_a;
        auto& b = collision.rb_b;

        auto m_inv = inverse_mass_matrix(a, b);

        gml::Matd<12, 1> q_pre{
                a.m_velocity.x(), a.m_velocity.y(), a.m_velocity.z(),
                b.m_velocity.x(), b.m_velocity.y(), b.m_velocity.z(),
                a.m_angular_velocity.x(), a.m_angular_velocity.y(), a.m_angular_velocity.z(),
                b.m_angular_velocity.x(), b.m_angular_velocity.y(), b.m_angular_velocity.z(),
        };

        auto& r_a = collision.contact_manifold.contacts.r_a;
        auto& r_b = collision.contact_manifold.contacts.r_b;
        auto& n = collision.contact_manifold.contacts.normal;
        auto v_rel = collision.rel_v;

        const double friction_coefficient = a.material.kinetic_friction * b.material.kinetic_friction;

        // Gram-Schmidt method using the relative velocity as the initial vector for the projection
        gml::Vec3d u1 = v_rel - n * gml::dot(v_rel, n); // non-normalized, since it might be zero-length
        gml::Vec3d u2;
        if (gml::sqrLength(u1) < 0.0000001) { // u1 is parallel to n, so we need another approach
            std::tie(u1, u2) = tangent_plane(n);
        } else {
            u1.normalize();
            u2 = gml::cross(u1, n); // normalization not necessary, since u1 and n are already normalized
        }

        // friction along u1
        auto t_1 = gml::cross(r_a, u1);
        auto t_2 = gml::cross(r_b, u1);
        gml::Matd<1, 12> j_f1{
                -u1.x(), -u1.y(), -u1.z(),
                u1.x(), u1.y(), u1.z(),
                -t_1.x(), -t_1.y(), -t_1.z(),
                t_2.x(), t_2.y(), t_2.z(),
        };
        auto j_f1_t = gml::transpose(j_f1);

        // accumulate impulses
        double old_lambda_f1 = collision.contact_manifold.lambda_f1;
        double lambda_f1 = solve(m_inv, j_f1, j_f1_t, q_pre, 0); // no positional bias for friction constraint
        collision.contact_manifold.lambda_f1 += lambda_f1;
        // clamp with accumulated normal impulse for the Coulomb friction model
        collision.contact_manifold.lambda_f1 = gml::clamp(
                collision.contact_manifold.lambda_f1,
                -friction_coefficient * collision.contact_manifold.lambda_n,
                friction_coefficient * collision.contact_manifold.lambda_n);
        // restore delta lambda after clamping
        lambda_f1 = collision.contact_manifold.lambda_f1 - old_lambda_f1;
        auto p_c = lambda_f1 * j_f1_t;

        // friction along u2
        t_1 = gml::cross(r_a, u2);
        t_2 = gml::cross(r_b, u2);
        gml::Matd<1, 12> j_f2{
                -u2.x(), -u2.y(), -u2.z(),
                u2.x(), u2.y(), u2.z(),
                -t_1.x(), -t_1.y(), -t_1.z(),
                t_2.x(), t_2.y(), t_2.z(),
        };
        auto j_f2_t = gml::transpose(j_f2);

        // accumulate impulses
        double old_lambda_f2 = collision.contact_manifold.lambda_f2;
        double lambda_f2 = solve(m_inv, j_f2, j_f2_t, q_pre, 0); // no positional bias for friction constraint
        collision.contact_manifold.lambda_f2 += lambda_f2;
        // clamp with accumulated normal impulse for the Coulomb friction model
        collision.contact_manifold.lambda_f2 = gml::clamp(
                collision.contact_manifold.lambda_f2 + lambda_f2,
                -friction_coefficient * collision.contact_manifold.lambda_n,
                friction_coefficient * collision.contact_manifold.lambda_n);
        // restore delta lambda after clamping
        lambda_f2 = collision.contact_manifold.lambda_f2 - old_lambda_f2;
        p_c += lambda_f2 * j_f2_t;

        auto q_post = q_pre + m_inv * p_c;
        a.m_velocity = {q_post(0, 0), q_post(1, 0), q_post(2, 0)};
        b.m_velocity = {q_post(3, 0), q_post(4, 0), q_post(5, 0)};
        a.m_angular_velocity = {q_post(6, 0), q_post(7, 0), q_post(8, 0)};
        b.m_angular_velocity = {q_post(9, 0), q_post(10, 0), q_post(11, 0)};
    }

    void Simulation::resolve_collision_rf(Collision& collision)
    {
        auto& a = collision.rb_a;
        auto& b = collision.rb_b;

        auto m_inv = inverse_mass_matrix(a, b);

        gml::Matd<12, 1> q_pre{
                a.m_velocity.x(), a.m_velocity.y(), a.m_velocity.z(),
                b.m_velocity.x(), b.m_velocity.y(), b.m_velocity.z(),
                a.m_angular_velocity.x(), a.m_angular_velocity.y(), a.m_angular_velocity.z(),
                b.m_angular_velocity.x(), b.m_angular_velocity.y(), b.m_angular_velocity.z(),
        };

        auto& n = collision.contact_manifold.contacts.normal;
        double spin_coefficient = 0.0005; // TODO: model like with friction

        gml::Matd<1, 12> j_spin{
                0, 0, 0,
                0, 0, 0,
                -n.x(), -n.y(), -n.z(),
                n.x(), n.y(), n.z(),
        };
        auto j_spin_t = gml::transpose(j_spin);

        // accumulate impulses
        double old_lambda_spin = collision.contact_manifold.lambda_rf_1;
        double lambda_spin = solve(m_inv, j_spin, j_spin_t, q_pre, 0);
        collision.contact_manifold.lambda_rf_1 += lambda_spin;
        // clamp to prevent reversing spin // TODO: clamp to zero in direction of sign
        // restore delta lambda after clamping
        lambda_spin = collision.contact_manifold.lambda_rf_1 - old_lambda_spin;
        auto p_c = spin_coefficient * lambda_spin * j_spin_t; // TODO: can we do something more specific?

        auto [u1, u2] = tangent_plane(n);
        gml::Matd<1, 12> j_u1{
                0, 0, 0,
                0, 0, 0,
                -u1.x(), -u1.y(), -u1.z(),
                u1.x(), u1.y(), u1.z(),
        };
        auto j_u1_t = gml::transpose(j_u1);

        // accumulate impulses
        double old_lambda_u1 = collision.contact_manifold.lambda_rf_2;
        double lambda_u1 = solve(m_inv, j_u1, j_u1_t, q_pre, 0);
        collision.contact_manifold.lambda_rf_2 += lambda_u1;
        // clamp to prevent reversing spin // TODO: clamp to zero in direction of sign
        // restore delta lambda after clamping
        lambda_u1 = collision.contact_manifold.lambda_rf_2 - old_lambda_u1;
        p_c += spin_coefficient * lambda_u1 * j_u1_t; // TODO: can we do something more specific?

        gml::Matd<1, 12> j_u2{
                0, 0, 0,
                0, 0, 0,
                -u2.x(), -u2.y(), -u2.z(),
                u2.x(), u2.y(), u2.z(),
        };
        auto j_u2_t = gml::transpose(j_u2);

        // accumulate impulses
        double old_lambda_u2 = collision.contact_manifold.lambda_rf_3;
        double lambda_u2 = solve(m_inv, j_u2, j_u2_t, q_pre, 0);
        collision.contact_manifold.lambda_rf_3 += lambda_u2;
        // clamp to prevent reversing spin // TODO: clamp to zero in direction of sign
        // restore delta lambda after clamping
        lambda_u2 = collision.contact_manifold.lambda_rf_3 - old_lambda_u2;
        p_c += spin_coefficient * lambda_u2 * j_u2_t; // TODO: can we do something more specific?

        auto q_post = q_pre + m_inv * p_c;
        a.m_velocity = {q_post(0, 0), q_post(1, 0), q_post(2, 0)};
        b.m_velocity = {q_post(3, 0), q_post(4, 0), q_post(5, 0)};
        a.m_angular_velocity = {q_post(6, 0), q_post(7, 0), q_post(8, 0)};
        b.m_angular_velocity = {q_post(9, 0), q_post(10, 0), q_post(11, 0)};
    }
#endif

    void Simulation::integrate(double dt)
    {
        for (auto& rigidBody: bodies) {
            // external forces
            if (rigidBody->m_inertia_shape.inverse_mass() > 0) {
                rigidBody->m_velocity += m_external_acceleration * dt;
            }

            // linear component
            rigidBody->m_velocity += rigidBody->m_inertia_shape.inverse_mass() * rigidBody->m_force * dt;

            // angular component
            rigidBody->m_angular_velocity +=
                    rigidBody->m_inertia_shape.inverse_inertia_tensor() * rigidBody->m_torque * dt;
        }

        std::vector<Constraint> penetration_constraints;
        std::vector<Constraint> friction_constraints;
        std::vector<Constraint> spinning_friction_constraints;
        std::vector<Constraint> rolling_friction_constraints;

        // narrow phase
        for (std::size_t i = 0; i < bodies.size() - 1; ++i) {
            auto& a = bodies.at(i);
            for (std::size_t j = i + 1; j < bodies.size(); ++j) {
                auto& b = bodies.at(j);

                auto result = std::visit(m_collision_visitor, a->m_bounding_volume, b->m_bounding_volume);
                if (result.has_value()) {
                    auto& manifold = result.value();
                    for (auto& contact: manifold.contacts) {
                        auto v_abs_p_a = a->m_velocity + gml::cross(a->m_angular_velocity, contact.r_a);
                        auto v_abs_p_b = b->m_velocity + gml::cross(b->m_angular_velocity, contact.r_b);
                        contact.rel_v = v_abs_p_b - v_abs_p_a;
                        contact.rel_v_n = gml::dot(contact.rel_v, manifold.normal);

                        // Gram-Schmidt method using the relative velocity as the initial vector for the projection
                        // don't normalize tangent here, since it might be zero-length
                        manifold.tangent_1 = contact.rel_v - manifold.normal * gml::dot(contact.rel_v, manifold.normal);
                        if (gml::sqrLength(manifold.tangent_1) < 0.0000001) {
                            // tangent is parallel to n, so we need another approach
                            std::tie(manifold.tangent_1, manifold.tangent_2) = tangent_plane(manifold.normal);
                        } else {
                            manifold.tangent_1.normalize();
                            // normalization not necessary, since tangent and normal are already normalized
                            manifold.tangent_2 = gml::cross(manifold.tangent_1,manifold.normal);
                        }

                        penetration_constraints.push_back(prepare_penetration_constraint(*a, *b, manifold, contact, dt));

                        auto [cf1, cf2] = prepare_friction_constraints(*a, *b, manifold, contact);
                        friction_constraints.push_back(cf1);
                        friction_constraints.push_back(cf2);

                        spinning_friction_constraints.push_back(prepare_spinning_friction_constraint(*a, *b, manifold));

                        auto [crf1, crf2] = prepare_rolling_friction_constraints(*a, *b, manifold);
                        rolling_friction_constraints.push_back(crf1);
                        rolling_friction_constraints.push_back(crf2);
                    }
                }
            }
        }

        // once all constraints are created, we can construct the dependencies
        for (std::size_t i = 0; i < friction_constraints.size(); ++i) {
            friction_constraints.at(i).dependent_constraint = {&penetration_constraints.at(i / 2)};
        }

        for (int i = 0; i < m_solver_iterations; ++i) {
            // don't interleave constraints, since the friction impulse depends on the normal impulse
            for (auto& constraint: penetration_constraints) {
                // accumulate impulses
                double old_lambda = constraint.accumulated_lambda;
                double delta_lambda = solve_constraint(constraint);
                // clamp to prevent objects pulling together for negative lambdas
                constraint.accumulated_lambda = std::max(0.0, constraint.accumulated_lambda + delta_lambda);
                // restore delta lambda after clamping
                delta_lambda = constraint.accumulated_lambda - old_lambda;
                auto delta_impulse = delta_lambda * constraint.j_t;
                apply_impulse(constraint, delta_impulse);
            }

            for (auto& constraint: friction_constraints) {
                // accumulate impulses
                double old_lambda = constraint.accumulated_lambda;
                double delta_lambda = solve_constraint(constraint);
                // clamp with accumulated normal impulse for the Coulomb friction model
                const double friction_coefficient =
                        constraint.rb_a.material.kinetic_friction *
                        constraint.rb_b.material.kinetic_friction;
                double lambda_n = constraint.dependent_constraint.value()->accumulated_lambda;
                constraint.accumulated_lambda = gml::clamp(
                        constraint.accumulated_lambda + delta_lambda,
                        -friction_coefficient * lambda_n,
                        friction_coefficient * lambda_n);
                // restore delta lambda after clamping
                delta_lambda = constraint.accumulated_lambda - old_lambda;
                auto delta_impulse = delta_lambda * constraint.j_t;
                apply_impulse(constraint, delta_impulse);
            }

            for (auto& constraint: spinning_friction_constraints) {
                // accumulate impulses
                double old_lambda = constraint.accumulated_lambda;
                double delta_lambda = solve_constraint(constraint);
                // clamp with accumulated normal impulse for the Coulomb friction model
                const double friction_coefficient =
                        constraint.rb_a.material.spinning_friction *
                        constraint.rb_b.material.spinning_friction;
                // TODO: clamp in zero direction preserving sign
                constraint.accumulated_lambda += delta_lambda;
                // restore delta lambda after clamping
                delta_lambda = constraint.accumulated_lambda - old_lambda;
                auto delta_impulse = friction_coefficient * delta_lambda * constraint.j_t;
                apply_impulse(constraint, delta_impulse);
            }

            for (auto& constraint: rolling_friction_constraints) {
                solve_constraint(constraint);
                // accumulate impulses
                double old_lambda = constraint.accumulated_lambda;
                double delta_lambda = solve_constraint(constraint);
                // clamp with accumulated normal impulse for the Coulomb friction model
                const double friction_coefficient =
                        constraint.rb_a.material.rolling_friction *
                        constraint.rb_b.material.rolling_friction;
                // TODO: clamp in zero direction preserving sign
                constraint.accumulated_lambda += delta_lambda;
                // restore delta lambda after clamping
                delta_lambda = constraint.accumulated_lambda - old_lambda;
                auto delta_impulse = friction_coefficient * delta_lambda * constraint.j_t;
                apply_impulse(constraint, delta_impulse);
            }
        }

        for (auto& rigidBody: bodies) {
            // linear component
            rigidBody->m_position += rigidBody->m_velocity * dt;
            rigidBody->m_force = gml::Vec3d();

            // angular component
            rigidBody->m_orientation += 0.5 * gml::Quatd(rigidBody->m_angular_velocity) * rigidBody->m_orientation * dt;
            rigidBody->m_orientation.normalize();
            rigidBody->m_torque = gml::Vec3d();

            rigidBody->update_bounding_volume();
        }
    }

    void Simulation::enable_gravity()
    {
        m_external_acceleration = {0, -9.81, 0};
    }

    Constraint Simulation::prepare_penetration_constraint(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold,
                                                          const ContactPoint& contact, double dt)
    {
        gml::Vec3d temp_1 = gml::cross(contact.r_a, manifold.normal);
        gml::Vec3d temp_2 = gml::cross(contact.r_b, manifold.normal);
        gml::Matd<1, 12> j{
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
                .j_t = gml::transpose(j),
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
        gml::Vec3d temp_1 = gml::cross(contact.r_a, manifold.tangent_1);
        gml::Vec3d temp_2 = gml::cross(contact.r_b, manifold.tangent_1);
        gml::Matd<1, 12> j_1{
                -manifold.tangent_1.x(), -manifold.tangent_1.y(), -manifold.tangent_1.z(),
                manifold.tangent_1.x(), manifold.tangent_1.y(), manifold.tangent_1.z(),
                -temp_1.x(), -temp_1.y(), -temp_1.z(),
                temp_2.x(), temp_2.y(), temp_2.z(),
        };

        // friction along second tangent
        temp_1 = gml::cross(contact.r_a, manifold.tangent_2);
        temp_2 = gml::cross(contact.r_b, manifold.tangent_2);
        gml::Matd<1, 12> j_2{
                -manifold.tangent_2.x(), -manifold.tangent_2.y(), -manifold.tangent_2.z(),
                manifold.tangent_2.x(), manifold.tangent_2.y(), manifold.tangent_2.z(),
                -temp_1.x(), -temp_1.y(), -temp_1.z(),
                temp_2.x(), temp_2.y(), temp_2.z(),
        };

        return {
                {
                        .m_inv = inverse_mass_matrix(rb_a, rb_b),
                        .j = j_1,
                        .j_t = gml::transpose(j_1),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                },
                {
                        .m_inv = inverse_mass_matrix(rb_a, rb_b),
                        .j = j_2,
                        .j_t = gml::transpose(j_2),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                }
        };
    }

    Constraint
    Simulation::prepare_spinning_friction_constraint(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold)
    {
        gml::Matd<1, 12> j{
                0, 0, 0,
                0, 0, 0,
                -manifold.normal.x(), -manifold.normal.y(), -manifold.normal.z(),
                manifold.normal.x(), manifold.normal.y(), manifold.normal.z(),
        };

        return {
                .m_inv = inverse_mass_matrix(rb_a, rb_b),
                .j = j,
                .j_t = gml::transpose(j),
                .rb_a = rb_a,
                .rb_b = rb_b,
        };
    }

    std::tuple<Constraint, Constraint>
    Simulation::prepare_rolling_friction_constraints(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold)
    {
        gml::Matd<1, 12> j_1{
                0, 0, 0,
                0, 0, 0,
                -manifold.tangent_1.x(), -manifold.tangent_1.y(), -manifold.tangent_1.z(),
                manifold.tangent_1.x(), manifold.tangent_1.y(), manifold.tangent_1.z(),
        };

        gml::Matd<1, 12> j_2{
                0, 0, 0,
                0, 0, 0,
                -manifold.tangent_2.x(), -manifold.tangent_2.y(), -manifold.tangent_2.z(),
                manifold.tangent_2.x(), manifold.tangent_2.y(), manifold.tangent_2.z(),
        };

        return {
                {
                        .m_inv = inverse_mass_matrix(rb_a, rb_b),
                        .j = j_1,
                        .j_t = gml::transpose(j_1),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                },
                {
                        .m_inv = inverse_mass_matrix(rb_a, rb_b),
                        .j = j_2,
                        .j_t = gml::transpose(j_2),
                        .rb_a = rb_a,
                        .rb_b = rb_b,
                }
        };
    }

    void Simulation::apply_impulse(const Constraint& constraint, gml::Matd<12, 1> impulse)
    {
        auto delta_q = constraint.m_inv * impulse;
        constraint.rb_a.m_velocity += {delta_q(0, 0), delta_q(1, 0), delta_q(2, 0)};
        constraint.rb_b.m_velocity += {delta_q(3, 0), delta_q(4, 0), delta_q(5, 0)};
        constraint.rb_a.m_angular_velocity += {delta_q(6, 0), delta_q(7, 0), delta_q(8, 0)};
        constraint.rb_b.m_angular_velocity += {delta_q(9, 0), delta_q(10, 0), delta_q(11, 0)};
    }
}

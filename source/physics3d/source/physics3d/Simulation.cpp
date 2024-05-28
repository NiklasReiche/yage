#include "Simulation.h"
#include <gml/quaternion.h>
#include <functional>

namespace physics3d
{
    gml::Matd<12, 12> Simulation::inverse_mass_matrix(RigidBody& a, RigidBody& b)
    {
        // TODO: instead of this sparse matrix multiplication, use the direct formula for the effective mass matrix
        gml::Matd<12, 12> m_inv;
        m_inv(0, 0) = a.inertia_shape.inverse_mass();
        m_inv(1, 1) = a.inertia_shape.inverse_mass();
        m_inv(2, 2) = a.inertia_shape.inverse_mass();
        m_inv(3, 3) = b.inertia_shape.inverse_mass();
        m_inv(4, 4) = b.inertia_shape.inverse_mass();
        m_inv(5, 5) = b.inertia_shape.inverse_mass();

        auto inv_inertia_a = a.inertia_shape.inverse_inertia_tensor();
        m_inv(6, 6) = inv_inertia_a(0, 0);
        m_inv(6, 7) = inv_inertia_a(0, 1);
        m_inv(6, 8) = inv_inertia_a(0, 2);
        m_inv(7, 6) = inv_inertia_a(1, 0);
        m_inv(7, 7) = inv_inertia_a(1, 1);
        m_inv(7, 8) = inv_inertia_a(1, 2);
        m_inv(8, 6) = inv_inertia_a(2, 0);
        m_inv(8, 7) = inv_inertia_a(2, 1);
        m_inv(8, 8) = inv_inertia_a(2, 2);

        auto inv_inertia_b = b.inertia_shape.inverse_inertia_tensor();
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

    double Simulation::solve(gml::Matd<12, 12> m_inv, gml::Matd<1, 12> j, gml::Matd<12, 1> j_t, gml::Matd<12, 1> q_pre)
    {
        auto nominator = -(j * q_pre);
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

    void Simulation::resolve_collision(const Collision& collision)
    {
        auto& a = collision.rb_a;
        auto& b = collision.rb_b;

        auto m_inv = inverse_mass_matrix(a, b);

        gml::Matd<12, 1> q_pre{
                a.velocity.x(), a.velocity.y(), a.velocity.z(),
                b.velocity.x(), b.velocity.y(), b.velocity.z(),
                a.angularVelocity.x(), a.angularVelocity.y(), a.angularVelocity.z(),
                b.angularVelocity.x(), b.angularVelocity.y(), b.angularVelocity.z(),
        };

        auto& r_a = collision.contact_manifold.contact.r_a;
        auto& r_b = collision.contact_manifold.contact.r_a;
        auto& p_a = collision.contact_manifold.contact.p_a;
        auto& p_b = collision.contact_manifold.contact.p_b;
        auto& n = collision.contact_manifold.contact.n;
        auto depth = gml::length(p_a - p_b);

        auto v_abs_p_a = a.velocity + gml::cross(a.angularVelocity, r_a);
        auto v_abs_p_b = b.velocity + gml::cross(b.angularVelocity, r_b);
        auto v_rel = v_abs_p_b - v_abs_p_a;

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
        gml::Matd<1, 12> j_penetration{
            -n.x(), -n.y(), -n.z(),
            n.x(), n.y(), n.z(),
            -t_1.x(), -t_1.y(), -t_1.z(),
            t_2.x(), t_2.y(), t_2.z(),
        };
        auto j_penetration_t = gml::transpose(j_penetration);
        double lambda_n = solve(m_inv, j_penetration, j_penetration_t, q_pre);
        auto p_c = lambda_n * j_penetration_t;

        // friction along u1
        const double friction_coefficient = 0.6;
        t_1 = gml::cross(r_a, u1);
        t_2 = gml::cross(r_b, u1);
        gml::Matd<1, 12> j_friction_1{
                -u1.x(), -u1.y(), -u1.z(),
                u1.x(), u1.y(), u1.z(),
                -t_1.x(), -t_1.y(), -t_1.z(),
                t_2.x(), t_2.y(), t_2.z(),
        };
        auto j_friction_1_t = gml::transpose(j_friction_1);
        double lambda_friction_1 = solve(m_inv, j_friction_1, j_friction_1_t, q_pre);
#if 0
        // TODO: clamping introduces lateral movement, is that expected?
        lambda_friction_1 = gml::clamp(lambda_friction_1,
                                       -friction_coefficient * lambda_n,
                                       friction_coefficient * lambda_n);
#endif
        p_c += lambda_friction_1 * j_friction_1_t;



        // friction along u2
        t_1 = gml::cross(r_a, u2);
        t_2 = gml::cross(r_b, u2);
        gml::Matd<1, 12> j_friction_2{
                -u2.x(), -u2.y(), -u2.z(),
                u2.x(), u2.y(), u2.z(),
                -t_1.x(), -t_1.y(), -t_1.z(),
                t_2.x(), t_2.y(), t_2.z(),
        };
        auto j_friction_2_t = gml::transpose(j_friction_2);
        double lambda_friction_2 = solve(m_inv, j_friction_2, j_friction_2_t, q_pre);
#if 0
        // TODO: clamping introduces lateral movement, is that expected?
        lambda_friction_2 = gml::clamp(lambda_friction_2,
                                       -friction_coefficient * lambda_n,
                                       friction_coefficient * lambda_n);
#endif
        p_c += lambda_friction_2 * j_friction_2_t;

        auto q_post = q_pre + m_inv * p_c;
        a.velocity = {q_post(0, 0), q_post(1, 0), q_post(2, 0)};
        b.velocity = {q_post(3, 0), q_post(4, 0), q_post(5, 0)};
        a.angularVelocity = {q_post(6, 0), q_post(7, 0), q_post(8, 0)};
        b.angularVelocity = {q_post(9, 0), q_post(10, 0), q_post(11, 0)};

        // TODO: try Baumgarte stabilisation instead
        a.position -= n * depth / 2;
        b.position += n * depth / 2;
        a.update_bounding_volume();
        b.update_bounding_volume();
    }

    void Simulation::integrate(double dt)
    {
        for (auto& particle: particles) {
            particle.velocity = particle.velocity + particle.force / particle.mass * dt;
            particle.position = particle.position + particle.velocity * dt;
            particle.force = gml::Vec3d(0, 0, 0);
        }


        for (auto& rigidBody: bodies) {
            // TODO: accumulate momentum or velocity? Probably doesn't matter
            // linear
            rigidBody->momentum = rigidBody->force * dt;
            rigidBody->velocity += rigidBody->inertia_shape.inverse_mass() * rigidBody->momentum;

            // angular component
            rigidBody->angularMomentum = rigidBody->torque * dt;
            rigidBody->angularVelocity += rigidBody->inertia_shape.inverse_inertia_tensor() * rigidBody->angularMomentum;
        }

        std::vector<Collision> collisions;

        // narrow phase
        for (std::size_t i = 0; i < bodies.size() - 1; ++i) {
            auto& a = bodies.at(i);
            for (std::size_t j = i + 1; j < bodies.size(); ++j) {
                auto& b = bodies.at(j);

                auto result = std::visit(m_collision_visitor, a->m_bounding_volume, b->m_bounding_volume);
                if (result.has_value()) {
                    Collision collision{
                            .contact_manifold = result.value(),
                            .rb_a = *a,
                            .rb_b = *b,
                    };
                    collisions.push_back(collision);
                }
            }
        }

        for (const auto& collision: collisions) {
            resolve_collision(collision);
        }


        for (auto& rigidBody: bodies) {
            // linear component
            rigidBody->position += rigidBody->velocity * dt;
            rigidBody->force = gml::Vec3d();

            // angular component
            rigidBody->orientation += 0.5 * gml::Quatd(rigidBody->angularVelocity) * rigidBody->orientation * dt;
            rigidBody->orientation.normalize();
            rigidBody->torque = gml::Vec3d();

            rigidBody->update_bounding_volume();
        }
    }

    Particle& Simulation::addParticle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass)
    {
        return particles.emplace_back(position, velocity, mass);
    }

    void Simulation::addRigidBody(const std::shared_ptr<RigidBody>& rigidBody)
    {
        bodies.emplace_back(rigidBody);
    }
}

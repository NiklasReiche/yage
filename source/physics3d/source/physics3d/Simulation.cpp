#include "Simulation.h"
#include <gml/quaternion.h>
#include <functional>

namespace physics3d
{
    void Simulation::resolve(RigidBody& a, RigidBody& b)
    {
        // linear impulse method
        const auto e = std::min(a.restitution, b.restitution);

        gml::Vec3d p_a;
        gml::Vec3d p_b;
        gml::Vec3d n;
        contact(a.bounding_shape, b.bounding_shape, p_a, p_b, n);
        auto depth = gml::length(p_a - p_b);

        auto nominator = gml::dot(-(1 + e) * (b.velocity - a.velocity), n);
        auto denominator = 1 / a.shape.mass + 1 / b.shape.mass;
        auto j = nominator / denominator;

        a.velocity -= j / a.shape.mass * n;
        b.velocity += j / b.shape.mass * n;

        a.position -= n * depth / 2;
        b.position += n * depth / 2;
        a.bounding_shape.center = a.position;
        b.bounding_shape.center = b.position;
    }

    void get_tangents(gml::Vec3d n, gml::Vec3d& p, gml::Vec3d& q)
    {
        if (std::abs(n(2)) > std::sqrt(0.5)){
            auto a = n(1) * n(1) + n(2) * n(2);
            auto k = std::sqrt(a);
            p(0) = 0;
            p(1) = -k*n(2);
            p(2) = -k*n(1);
            q(0) = a*k;
            q(1) = -n(0)*p(2);
            q(2) = n(0)*p(1);
        } else {
            auto a = n(0) * n(0) + n(1) * n(1);
            auto k = std::sqrt(a);
            p(0) = -k * n(1);
            p(1) = -k * n(0);
            p(2) = 0;
            q(0) = -n(2) * p(1);
            q(1) = n(2) * p(2);
            q(2) = a * k;
        }
    }

    gml::Matd<12, 12> Simulation::inverse_mass_matrix(RigidBody& a, RigidBody& b)
    {
        gml::Matd<12, 12> m_inv;
        m_inv(0, 0) = 1.0 / a.shape.mass;
        m_inv(1, 1) = 1.0 / a.shape.mass;
        m_inv(2, 2) = 1.0 / a.shape.mass;
        m_inv(3, 3) = 1.0 / b.shape.mass;
        m_inv(4, 4) = 1.0 / b.shape.mass;
        m_inv(5, 5) = 1.0 / b.shape.mass;

        m_inv(6, 6) = a.shape.inverseInertiaTensor(0, 0);
        m_inv(6, 7) = a.shape.inverseInertiaTensor(0, 1);
        m_inv(6, 8) = a.shape.inverseInertiaTensor(0, 2);
        m_inv(7, 6) = a.shape.inverseInertiaTensor(1, 0);
        m_inv(7, 7) = a.shape.inverseInertiaTensor(1, 1);
        m_inv(7, 8) = a.shape.inverseInertiaTensor(1, 2);
        m_inv(8, 6) = a.shape.inverseInertiaTensor(2, 0);
        m_inv(8, 7) = a.shape.inverseInertiaTensor(2, 1);
        m_inv(8, 8) = a.shape.inverseInertiaTensor(2, 2);

        m_inv(9, 9) = b.shape.inverseInertiaTensor(0, 0);
        m_inv(9, 10) = b.shape.inverseInertiaTensor(0, 1);
        m_inv(9, 11) = b.shape.inverseInertiaTensor(0, 2);
        m_inv(10, 9) = b.shape.inverseInertiaTensor(1, 0);
        m_inv(10, 10) = b.shape.inverseInertiaTensor(1, 1);
        m_inv(10, 11) = b.shape.inverseInertiaTensor(1, 2);
        m_inv(11, 9) = b.shape.inverseInertiaTensor(2, 0);
        m_inv(11, 10) = b.shape.inverseInertiaTensor(2, 1);
        m_inv(11, 11) = b.shape.inverseInertiaTensor(2, 2);

        return m_inv;
    }

    void Simulation::solve(gml::Matd<12, 12> m_inv, gml::Matd<1, 12> j, gml::Matd<12, 1> q_pre,
                           gml::Matd<12, 1>& delta_q, double& lambda,
                           const std::function<double(double)>& lambda_clamp)
    {
        auto j_t = gml::transpose(j);

        auto nominator = -(j * q_pre);
        auto denominator = j * m_inv * j_t;
        lambda = nominator(0, 0) / denominator(0, 0);

        lambda = lambda_clamp(lambda);

        auto p_c = lambda * j_t;
        delta_q = m_inv * p_c;
    }

    void Simulation::resolve_constraint(RigidBody& a, RigidBody& b)
    {
        auto m_inv = inverse_mass_matrix(a, b);

        gml::Matd<12, 1> q_pre{
                a.velocity.x(), a.velocity.y(), a.velocity.z(),
                b.velocity.x(), b.velocity.y(), b.velocity.z(),
                a.angularVelocity.x(), a.angularVelocity.y(), a.angularVelocity.z(),
                b.angularVelocity.x(), b.angularVelocity.y(), b.angularVelocity.z(),
        };

        gml::Vec3d p_a;
        gml::Vec3d p_b;
        gml::Vec3d n;
        contact(a.bounding_shape, b.bounding_shape, p_a, p_b, n);
        auto depth = gml::length(p_a - p_b);
        gml::Vec3d u1;
        gml::Vec3d u2;
        get_tangents(n, u1, u2);
        u1.normalize();
        u2.normalize();
        auto r_a = p_a - a.bounding_shape.center;
        auto r_b = p_b - b.bounding_shape.center;

        auto t_1 = gml::cross(r_a, n);
        auto t_2 = gml::cross(r_b, n);
        gml::Matd<1, 12> j_penetration{
            -n.x(), -n.y(), -n.z(),
            n.x(), n.y(), n.z(),
            -t_1.x(), -t_1.y(), -t_1.z(),
            t_2.x(), t_2.y(), t_2.z(),
        };

        t_1 = -gml::cross(r_a, u1); // the sign is due to handedness of the cross product
        t_2 = -gml::cross(r_b, u1); // the sign is due to handedness of the cross product
        gml::Matd<1, 12> j_friction_1{
                -u1.x(), -u1.y(), -u1.z(),
                u1.x(), u1.y(), u1.z(),
                -t_1.x(), -t_1.y(), -t_1.z(),
                t_2.x(), t_2.y(), t_2.z(),
        };

        t_1 = -gml::cross(r_a, u2); // the sign is due to handedness of the cross product
        t_2 = -gml::cross(r_b, u2); // the sign is due to handedness of the cross product
        gml::Matd<1, 12> j_friction_2{
                -u2.x(), -u2.y(), -u2.z(),
                u2.x(), u2.y(), u2.z(),
                -t_1.x(), -t_1.y(), -t_1.z(),
                t_2.x(), t_2.y(), t_2.z(),
        };

        gml::Matd<12, 1> delta_q_penetration;
        double lambda_n;
        solve(m_inv, j_penetration, q_pre, delta_q_penetration, lambda_n);

        const double friction_constant = 0.8;
        auto clamp_lambda = [lambda_n, friction_constant](double l){
            return gml::clamp(l, -friction_constant * lambda_n, friction_constant * lambda_n);
        };

        gml::Matd<12, 1> delta_q_friction_1;
        double lambda_friction_1;
        solve(m_inv, j_friction_1, q_pre, delta_q_friction_1, lambda_friction_1, clamp_lambda);
        gml::Matd<12, 1> delta_q_friction_2;
        double lambda_friction_2;
        solve(m_inv, j_friction_2, q_pre, delta_q_friction_2,lambda_friction_2, clamp_lambda);

        auto q_post = q_pre + delta_q_penetration + delta_q_friction_1 + delta_q_friction_2;

        a.velocity = {q_post(0, 0), q_post(1, 0), q_post(2, 0)};
        b.velocity = {q_post(3, 0), q_post(4, 0), q_post(5, 0)};
        a.angularVelocity = {q_post(6, 0), q_post(7, 0), q_post(8, 0)};
        b.angularVelocity = {q_post(9, 0), q_post(10, 0), q_post(11, 0)};

        // TODO: try Baumgarte stabilisation instead
        a.position -= n * depth / 2;
        b.position += n * depth / 2;
        a.bounding_shape.center = a.position;
        b.bounding_shape.center = b.position;
    }

    void Simulation::integrate(double dt)
    {
        for (auto& particle: particles) {
            particle.velocity = particle.velocity + particle.force / particle.mass * dt;
            particle.position = particle.position + particle.velocity * dt;
            particle.force = gml::Vec3d(0, 0, 0);
        }


        for (auto& rigidBody: bodies) {
            // TODO: accumulate momentum or velocity?
            // linear
            rigidBody->momentum = rigidBody->force * dt;
            rigidBody->velocity += rigidBody->momentum / rigidBody->shape.mass;

            // angular component
            rigidBody->angularMomentum = rigidBody->torque * dt; // TODO: accumulate momentum or velocity
            rigidBody->angularVelocity += rigidBody->shape.inverseInertiaTensor * rigidBody->angularMomentum;
        }


        for (std::size_t i = 0; i < bodies.size() - 1; ++i) {
            auto& a = bodies.at(i);
            for (std::size_t j = i + 1; j < bodies.size(); ++j) {
                auto& b = bodies.at(j);
                if (collide(a->bounding_shape, b->bounding_shape)) {
                    resolve_constraint(*a, *b);
                }
            }
        }


        for (auto& rigidBody: bodies) {
            // linear component
            rigidBody->position += rigidBody->velocity * dt;
            rigidBody->force = gml::Vec3d();
            rigidBody->bounding_shape.center = rigidBody->position;

            // angular component
            rigidBody->orientation += 0.5 * gml::Quatd(rigidBody->angularVelocity) * rigidBody->orientation;
            rigidBody->orientation.normalize();
            rigidBody->torque = gml::Vec3d();
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

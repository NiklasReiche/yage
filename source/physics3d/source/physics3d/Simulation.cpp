#include "Simulation.h"
#include <gml/quaternion.h>

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

    void Simulation::resolve_constraint(RigidBody& a, RigidBody& b)
    {
        gml::Vec3d p_a;
        gml::Vec3d p_b;
        gml::Vec3d n;
        contact(a.bounding_shape, b.bounding_shape, p_a, p_b, n);
        auto depth = gml::length(p_a - p_b);

        auto m_inv = gml::matrix::diagonal<double, 6>({
            1.0 / a.shape.mass, 1.0 / a.shape.mass, 1.0 / a.shape.mass,
            1.0 / b.shape.mass, 1.0 / b.shape.mass, 1.0 / b.shape.mass,
        });

        gml::Matd<1, 6> j{
            -n.x(), -n.y(), -n.z(), n.x(), n.y(), n.z()
        };
        auto j_trans = gml::transpose(j);

        gml::Matd<6, 1> q_pre{
            a.velocity.x(), a.velocity.y(), a.velocity.z(),
            b.velocity.x(), b.velocity.y(), b.velocity.z(),
        };

        auto nominator = -(j * q_pre);
        auto denominator = j * m_inv * j_trans;
        auto lambda = nominator(0, 0) / denominator(0, 0);

        auto p_c = lambda * j_trans;
        auto q_post = q_pre + (m_inv * p_c);

        a.velocity = {q_post(0, 0), q_post(1, 0), q_post(2, 0)};
        b.velocity = {q_post(3, 0), q_post(4, 0), q_post(5, 0)};

        // TODO
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
            rigidBody->angularMomentum += rigidBody->torque * dt; // TODO: accumulate momentum or velocity
            rigidBody->angularVelocity = rigidBody->shape.inverseInertiaTensor * rigidBody->angularMomentum;
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

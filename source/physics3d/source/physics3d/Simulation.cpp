#include "Simulation.h"
#include <gml/quaternion.h>

namespace physics3d
{
	void Simulation::integrate(double dt)
	{
		for (auto& particle : particles) {
			particle.velocity = particle.velocity + particle.force / particle.mass * dt;
			particle.position = particle.position + particle.velocity * dt;
			particle.force = gml::Vec3d(0, 0, 0);
		}

		for (auto& rigidBody : bodies) {
			// linear component
			rigidBody->momentum += rigidBody->force * dt;
			rigidBody->velocity = rigidBody->momentum / rigidBody->shape.mass;
			rigidBody->position += rigidBody->velocity * dt;
			rigidBody->force = gml::Vec3d();

			// angular component
			rigidBody->angularMomentum += rigidBody->torque * dt;
			rigidBody->angularVelocity = rigidBody->shape.inverseInertiaTensor * rigidBody->angularMomentum;
			rigidBody->orientation += 0.5 * gml::Quatd(rigidBody->angularVelocity) * rigidBody->orientation;
			rigidBody->orientation.normalize();
			rigidBody->torque = gml::Vec3d();
		}
	}

	Particle & Simulation::addParticle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass)
	{
		return particles.emplace_back(position, velocity, mass);
	}

	void Simulation::addRigidBody(const std::shared_ptr<RigidBody>& rigidBody)
	{
		bodies.emplace_back(rigidBody);
	}
}

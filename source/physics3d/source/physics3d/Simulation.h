#pragma once

#include <vector>

#include "Particle.h"
#include "RigidBody.h"

namespace physics3d
{
	class Simulation
	{
	public:
		void integrate(double dt);

		Particle& addParticle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass);

		void addRigidBody(const std::shared_ptr<RigidBody>& rigidBody);

	private:
		std::vector<Particle> particles;
		std::vector<std::shared_ptr<RigidBody>> bodies;
	};
}

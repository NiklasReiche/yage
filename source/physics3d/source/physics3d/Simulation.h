#pragma once

#include <vector>

#include "Particle.h"

namespace physics3d
{
	class Simulation
	{
	public:
		void integrate(double dt);

		Particle& addParticle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass);

	private:
		std::vector<Particle> particles;
	};
}

#include "Simulation.h"

namespace physics3d
{
	void Simulation::integrate(double dt)
	{
		for (auto& particle : particles) {
			particle.velocity = particle.velocity + particle.force / particle.mass * dt;
			particle.position = particle.position + particle.velocity * dt;
			particle.force = gml::Vec3d(0, 0, 0);
		}
	}

	Particle & Simulation::addParticle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass)
	{
		return particles.emplace_back(position, velocity, mass);
	}
}

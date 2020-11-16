#pragma once

#include <gml/vector.h>

namespace physics3d
{
	class Particle
	{
	public:
		Particle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass);

		void applyForce(const gml::Vec3d& additiveForce);

		gml::Vec3d getPosition();

	private:
		gml::Vec3d position;
		gml::Vec3d velocity;
		double mass;

		gml::Vec3d force;

		friend class Simulation;
	};
}

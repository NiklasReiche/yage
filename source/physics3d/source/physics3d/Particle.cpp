#include "Particle.h"

namespace physics3d
{
	Particle::Particle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass)
		: position(position), velocity(velocity), mass(mass)
	{ }

	void Particle::applyForce(const gml::Vec3d& additiveForce)
	{
		force += additiveForce;
	}

	gml::Vec3d Particle::getPosition()
	{
		return position;
	}
}

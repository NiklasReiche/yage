#pragma once

#include <memory>

#include <gml/vector.h>
#include <gml/quaternion.h>

namespace physics3d
{
	class RigidBody
	{
	public:
		RigidBody(double mass, double inertia);

		void applyForce(const gml::Vec3d& _force, const gml::Vec3d& point);

		[[nodiscard]]
		gml::Vec3d getPosition();

		[[nodiscard]]
		gml::Quatd getOrientation();

	private:
		gml::Vec3d position;
		gml::Vec3d momentum;
		gml::Quatd orientation;
		gml::Vec3d angularMomentum;

		gml::Vec3d velocity;
		gml::Vec3d angularVelocity;
		gml::Quatd spin;

		gml::Vec3d force;
		gml::Vec3d torque;

		double mass;
		double inertia;

		friend class Simulation;
	};
}

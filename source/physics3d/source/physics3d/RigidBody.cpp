#include "RigidBody.h"

namespace physics3d
{
	RigidBody::RigidBody(const InertiaShape &shape, const gml::Vec3d &position, const gml::Quatd &orientation)
		: position(position), orientation(orientation), shape(shape)
	{

	}

	void RigidBody::applyForce(const gml::Vec3d& _force, const gml::Vec3d& point)
	{
		this->force += _force;
		torque += gml::cross(_force, gml::Vec3d(point - position));
	}

	gml::Vec3d RigidBody::getPosition()
	{
		return position;
	}

	gml::Quatd RigidBody::getOrientation()
	{
		return orientation;
	}
}

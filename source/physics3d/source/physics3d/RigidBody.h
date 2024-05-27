#pragma once

#include <memory>

#include <gml/vector.h>
#include <gml/quaternion.h>

#include "InertiaShape.h"
#include "BoundingShape.h"

namespace physics3d
{
	class RigidBody
	{
	public:
		RigidBody(const InertiaShape& shape, const gml::Vec3d& position, const gml::Quatd& orientation);

		void applyForce(const gml::Vec3d& _force, const gml::Vec3d& point);

		[[nodiscard]]
		gml::Vec3d getPosition();

		[[nodiscard]]
		gml::Quatd getOrientation();

        void update_bounding_shape();

        BoundingSphere bounding_shape{
                .radius = 1
        };

	private:
		gml::Vec3d position;
		gml::Vec3d momentum;
		gml::Quatd orientation;
		gml::Vec3d angularMomentum;

		gml::Vec3d velocity;
		gml::Vec3d angularVelocity;

		gml::Vec3d force;
		gml::Vec3d torque;

		InertiaShape inertia_shape;



		friend class Simulation;
	};
}

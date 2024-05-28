#pragma once

#include <memory>

#include <gml/vector.h>
#include <gml/quaternion.h>
#include <variant>

#include "InertiaShape.h"
#include "BoundingShape.h"

namespace physics3d
{
    using BoundingVolume = std::variant<BSphere, BPlane>;

	class RigidBody
	{
	public:
		RigidBody(const InertiaShape& shape,
                  const BoundingVolume& bounding_volume,
                  const gml::Vec3d& position,
                  const gml::Quatd& orientation,
                  const gml::Vec3d& bounding_volume_offset = {});

		void applyForce(const gml::Vec3d& _force, const gml::Vec3d& point);

		[[nodiscard]]
		gml::Vec3d getPosition() const;

		[[nodiscard]]
		gml::Quatd getOrientation() const;

        void update_bounding_volume();

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

        BoundingVolume m_bounding_volume;
        gml::Vec3d m_bounding_volume_offset{};

		friend class Simulation;
	};
}

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

    /**
     * Contains properties related to bounciness and friction for rigid bodies.
     */
    struct Material
    {
        double restitution{};
        double kinetic_friction{};
        double spinning_friction{};
        double rolling_friction{};
    };

    class RigidBody
    {
    public:
        Material material;

        RigidBody(const InertiaShape& inertia_shape,
                  const BoundingVolume& bounding_volume,
                  const Material& material,
                  const gml::Vec3d& initial_position,
                  const gml::Quatd& initial_orientation,
                  const gml::Vec3d& bounding_volume_offset = {});

        /**
         * Applies a force at a given point.
         * @param force the force in newtons
         * @param point the attack point in world space
         */
        void apply_force(const gml::Vec3d& force, const gml::Vec3d& point);

        void destroy();

        [[nodiscard]]
        gml::Vec3d position() const;

        [[nodiscard]]
        gml::Quatd orientation() const;

        [[nodiscard]]
        gml::Vec3d velocity() const;

        [[nodiscard]]
        gml::Vec3d angular_velocity() const;

        [[nodiscard]]
        gml::Vec3d force() const;

        [[nodiscard]]
        gml::Vec3d torque() const;

    private:
        gml::Vec3d m_position;
        gml::Quatd m_orientation;

        gml::Vec3d m_velocity;
        gml::Vec3d m_angular_velocity;

        gml::Vec3d m_force;
        gml::Vec3d m_torque;

        InertiaShape m_inertia_shape;

        BoundingVolume m_bounding_volume;
        gml::Vec3d m_bounding_volume_offset;

        bool m_should_destroy{};

        void update_bounding_volume();

        friend class Simulation;
    };
}

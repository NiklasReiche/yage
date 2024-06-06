#pragma once

#include <memory>

#include <gml/vector.h>
#include <gml/quaternion.h>

#include "InertiaShape.h"
#include "BoundingShape.h"

namespace physics3d
{
    /**
     * Contains properties related to bounciness and friction for rigid bodies.
     */
    struct Material
    {
        double restitution{};
        double kinetic_friction{};
        double rolling_friction{};
    };

    class RigidBody
    {
    public:
        Material material;

        RigidBody(const InertiaShape& inertia_shape,
                  const Collider& bounding_volume,
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

        /**
         * Marks this rigid body for destruction before the next simulation step. While the object itself is not
         * destroyed, it does get removed from the simulation.
         */
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

        std::optional<Collider> m_collider;
        gml::Vec3d m_collider_offset;

        bool m_should_destroy{};

        void update_collider();

        friend class Simulation;
    };
}

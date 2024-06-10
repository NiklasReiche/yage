#pragma once

#include <math/vector.h>
#include <math/quaternion.h>

#include "InertiaShape.h"
#include "BoundingShape.h"

namespace yage::physics3d
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
                  const math::Vec3d& initial_position,
                  const math::Quatd& initial_orientation,
                  const math::Vec3d& bounding_volume_offset = {});

        /**
         * Applies a force at a given point.
         * @param force the force in newtons
         * @param point the attack point in world space
         */
        void apply_force(const math::Vec3d& force, const math::Vec3d& point);

        /**
         * Marks this rigid body for destruction before the next simulation step. While the object itself is not
         * destroyed, it does get removed from the simulation.
         */
        void destroy();

        [[nodiscard]]
        math::Vec3d position() const;

        [[nodiscard]]
        math::Quatd orientation() const;

        [[nodiscard]]
        math::Vec3d velocity() const;

        [[nodiscard]]
        math::Vec3d angular_velocity() const;

        [[nodiscard]]
        math::Vec3d force() const;

        [[nodiscard]]
        math::Vec3d torque() const;

    private:
        math::Vec3d m_position;
        math::Quatd m_orientation;

        math::Vec3d m_velocity;
        math::Vec3d m_angular_velocity;

        math::Vec3d m_force;
        math::Vec3d m_torque;

        InertiaShape m_inertia_shape;

        std::optional<Collider> m_collider;
        math::Vec3d m_collider_offset;

        bool m_should_destroy{};

        void update_collider();

        friend class Simulation;
    };
}

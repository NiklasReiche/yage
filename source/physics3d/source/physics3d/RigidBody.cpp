#include "RigidBody.h"
#include <utils/utils.h>

namespace yage::physics3d
{
    RigidBody::RigidBody(const InertiaShape& inertia_shape,
                         const Collider& bounding_volume,
                         const Material& material,
                         const math::Vec3d& initial_position,
                         const math::Quatd& initial_orientation,
                         const math::Vec3d& bounding_volume_offset)
            : material(material),
              m_position(initial_position),
              m_orientation(initial_orientation),
              m_inertia_shape(inertia_shape),
              m_collider(bounding_volume),
              m_collider_offset(bounding_volume_offset)
    {
        update_collider();
    }

    void RigidBody::apply_force(const math::Vec3d& force, const math::Vec3d& point)
    {
        m_force += force;
        m_torque += cross(force, math::Vec3d(point - m_position));
    }

    void RigidBody::update_collider()
    {
        if (!m_collider.has_value())
            return;

        std::visit(utils::overload{
                [this](colliders::Sphere& sphere) {
                    sphere.center = m_position + m_collider_offset;
                },
                [this](colliders::OrientedPlane& plane) {
                    plane.support = m_position + m_collider_offset;
                    plane.normal = m_orientation * plane.original_normal;
                },
                [this](colliders::OrientedBox& box) {
                    box.center = m_position + m_collider_offset;
                    box.orientation = m_orientation;
                    box.update_computed_values();
                },
        }, m_collider.value());
    }

    math::Vec3d RigidBody::position() const
    {
        return m_position;
    }

    math::Quatd RigidBody::orientation() const
    {
        return m_orientation;
    }

    math::Vec3d RigidBody::velocity() const
    {
        return m_velocity;
    }

    math::Vec3d RigidBody::angular_velocity() const
    {
        return m_angular_velocity;
    }

    math::Vec3d RigidBody::force() const
    {
        return m_force;
    }

    math::Vec3d RigidBody::torque() const
    {
        return m_torque;
    }

    void RigidBody::destroy()
    {
        m_should_destroy = true;
    }
}

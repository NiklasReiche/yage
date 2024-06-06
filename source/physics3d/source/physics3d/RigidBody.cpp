#include "RigidBody.h"
#include <utils/utils.h>

namespace physics3d
{
    RigidBody::RigidBody(const InertiaShape& inertia_shape,
                         const Collider& bounding_volume,
                         const Material& material,
                         const gml::Vec3d& position,
                         const gml::Quatd& orientation,
                         const gml::Vec3d& bounding_volume_offset)
            : material(material),
              m_position(position),
              m_orientation(orientation),
              m_inertia_shape(inertia_shape),
              m_collider(bounding_volume),
              m_collider_offset(bounding_volume_offset)
    {
        update_collider();
    }

    void RigidBody::apply_force(const gml::Vec3d& force, const gml::Vec3d& point)
    {
        m_force += force;
        m_torque += gml::cross(force, gml::Vec3d(point - m_position));
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
                },
        }, m_collider.value());
    }

    gml::Vec3d RigidBody::position() const
    {
        return m_position;
    }

    gml::Quatd RigidBody::orientation() const
    {
        return m_orientation;
    }

    gml::Vec3d RigidBody::velocity() const
    {
        return m_velocity;
    }

    gml::Vec3d RigidBody::angular_velocity() const
    {
        return m_angular_velocity;
    }

    gml::Vec3d RigidBody::force() const
    {
        return m_force;
    }

    gml::Vec3d RigidBody::torque() const
    {
        return m_torque;
    }

    void RigidBody::destroy()
    {
        m_should_destroy = true;
    }
}

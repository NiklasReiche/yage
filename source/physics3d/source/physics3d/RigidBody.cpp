#include "RigidBody.h"
#include <utils/utils.h>

namespace physics3d
{
    RigidBody::RigidBody(const InertiaShape& shape,
                         const BoundingVolume& bounding_volume,
                         const gml::Vec3d& position,
                         const gml::Quatd& orientation,
                         const gml::Vec3d& bounding_volume_offset)
            : position(position), orientation(orientation), inertia_shape(shape), m_bounding_volume(bounding_volume),
              m_bounding_volume_offset(bounding_volume_offset)
    {
        update_bounding_volume();
    }

    void RigidBody::applyForce(const gml::Vec3d& _force, const gml::Vec3d& point)
    {
        this->force += _force;
        torque += gml::cross(_force, gml::Vec3d(point - position));
    }

    gml::Vec3d RigidBody::getPosition() const
    {
        return position;
    }

    gml::Quatd RigidBody::getOrientation() const
    {
        return orientation;
    }

    void RigidBody::update_bounding_volume()
    {
        std::visit(utils::overload{
                [this](BSphere& sphere) { sphere.center = position + m_bounding_volume_offset; },
                [this](BPlane& plane) { plane.support = position + m_bounding_volume_offset; },
        }, m_bounding_volume);
    }
}

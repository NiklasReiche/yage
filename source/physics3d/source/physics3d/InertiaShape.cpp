#include "InertiaShape.h"

namespace yage::physics3d
{
    InertiaShape::InertiaShape(const double inverse_mass, const math::Mat3d& inverse_inertia_tensor)
        : m_inverse_mass(inverse_mass), m_inverse_inertia_tensor(inverse_inertia_tensor)
    {
    }

    double InertiaShape::inverse_mass() const
    {
        return m_inverse_mass;
    }

    math::Mat3d InertiaShape::inverse_inertia_tensor() const
    {
        return m_inverse_inertia_tensor;
    }

    InertiaShape InertiaShape::static_shape()
    {
        return {0, math::Mat3d(0)};
    }

    InertiaShape InertiaShape::sphere(const double radius, const double mass)
    {
        const double moment = mass * radius * radius * 2. / 5.;
        return {
            1. / mass,
            math::Mat3d{
                1. / moment, 0, 0,
                0, 1. / moment, 0,
                0, 0, 1. / moment
            }
        };
    }

    InertiaShape InertiaShape::cube(const double length, const double mass)
    {
        const double moment = mass * length * length / 6.;
        return {
            1. / mass,
            math::Mat3d{
                1. / moment, 0, 0,
                0, 1. / moment, 0,
                0, 0, 1. / moment
            }
        };
    }

    InertiaShape InertiaShape::cuboid(const double width, const double height, const double depth, const double mass)
    {
        return {
            1. / mass,
            math::Mat3d{
                1. / mass * (height * height + depth * depth) / 12., 0, 0,
                0, 1. / mass * (width * width + height * height) / 12., 0,
                0, 0, 1. / mass * (width * width + depth * depth) / 12.
            }
        };
    }
}

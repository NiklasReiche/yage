#include "InertiaShape.h"

namespace yage::physics3d
{
    InertiaShape::InertiaShape(double inverse_mass, math::Mat3d inverse_inertia_tensor)
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

    InertiaShape InertiaShape::sphere(double radius, double mass)
    {
        double moment = mass * radius * radius * 2. / 5.;
        return InertiaShape(
                1. / mass,
                math::Mat3d{
                        1. / moment, 0, 0,
                        0, 1. / moment, 0,
                        0, 0, 1. / moment
                });
    }

    InertiaShape InertiaShape::cube(double length, double mass)
    {
        double moment = mass * length * length / 6.;
        return InertiaShape(
                1. / mass,
                math::Mat3d{
                        1. / moment, 0, 0,
                        0, 1. / moment, 0,
                        0, 0, 1. / moment
                });
    }

    InertiaShape InertiaShape::cuboid(double width, double height, double depth, double mass)
    {
        return InertiaShape(
                1. / mass,
                math::Mat3d{
                        1. / mass * (height * height + depth * depth) / 12., 0, 0,
                        0, 1. / mass * (width * width + height * height) / 12., 0,
                        0, 0, 1. / mass * (width * width + depth * depth) / 12.
                });
    }
}

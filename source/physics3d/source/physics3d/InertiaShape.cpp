#include "InertiaShape.h"

namespace physics3d
{
    double InertiaShape::inverse_mass() const
    {
        return m_inverse_mass;
    }

    gml::Mat3d InertiaShape::inverse_inertia_tensor() const
    {
        return m_inverse_inertia_tensor;
    }

    StaticShape::StaticShape()
    {
        m_inverse_mass = 0;
        m_inverse_inertia_tensor = gml::Mat3d(0);
    }

    CubeShape::CubeShape(double length, double mass)
    {
        m_inverse_mass = 1. / mass;

        double moment = mass * length * length / 6.;
        m_inverse_inertia_tensor = {
                1. / moment, 0, 0,
                0, 1. / moment, 0,
                0, 0, 1. / moment
        };
    }

    BoxShape::BoxShape(double width, double height, double depth, double mass)
    {
        m_inverse_mass = 1. / mass;

        m_inverse_inertia_tensor = {
                1. / mass * (height * height + depth * depth) / 12., 0, 0,
                0, 1. / mass * (width * width + height * height) / 12., 0,
                0, 0, 1. / mass * (width * width + depth * depth) / 12.
        };
    }

    SphereShape::SphereShape(double radius, double mass)
    {
        m_inverse_mass = 1. / mass;

        double moment = mass * radius * radius * 2. / 5.;
        m_inverse_inertia_tensor = {
                1. / moment, 0, 0,
                0, 1. / moment, 0,
                0, 0, 1. / moment
        };
    }
}

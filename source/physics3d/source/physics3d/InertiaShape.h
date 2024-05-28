#pragma once

#include <gml/matrix.h>

namespace physics3d
{
    class InertiaShape
    {
    public:
        [[nodiscard]] double inverse_mass() const;

        [[nodiscard]] gml::Mat3d inverse_inertia_tensor() const;

    protected:
        InertiaShape() = default;

        double m_inverse_mass = 1;
        gml::Mat3d m_inverse_inertia_tensor;
    };

    struct StaticShape : public InertiaShape
    {
        StaticShape();
    };

    struct CubeShape : public InertiaShape
    {
        CubeShape(double length, double mass);
    };

    struct BoxShape : public InertiaShape
    {
        BoxShape(double width, double height, double depth, double mass);
    };

    struct SphereShape : public InertiaShape
    {
        SphereShape(double radius, double mass);
    };
}

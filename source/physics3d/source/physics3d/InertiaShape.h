#pragma once

#include <gml/matrix.h>

namespace physics3d
{
    struct InertiaShape
    {
        double mass = 1;
        gml::Mat3d inertiaTensor;
        gml::Mat3d inverseInertiaTensor;
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

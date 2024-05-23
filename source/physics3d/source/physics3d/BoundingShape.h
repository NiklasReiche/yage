#pragma once

#include "gml/vector.h"

namespace physics3d
{
    class BoundingShape
    {

    };

    class BoundingSphere : public BoundingShape
    {
    public:
        gml::Vec3d center{};
        double radius{};
    };

    bool collide(BoundingSphere a, BoundingSphere b)
    {
        return gml::sqrLength(a.center - b.center) <= (a.radius + b.radius) * (a.radius + b.radius);
    }
}

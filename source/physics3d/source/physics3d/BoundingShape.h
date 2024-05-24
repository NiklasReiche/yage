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

    inline bool collide(BoundingSphere a, BoundingSphere b)
    {
        return gml::sqrLength(a.center - b.center) <= (a.radius + b.radius) * (a.radius + b.radius);
    }

    inline void contact(BoundingSphere a, BoundingSphere b, gml::Vec3d& p_a, gml::Vec3d& p_b, gml::Vec3d& n)
    {
        n = gml::normalize(b.center - a.center);
        p_a = a.center + n * a.radius;
        p_b = b.center - n * b.radius;
    }


}

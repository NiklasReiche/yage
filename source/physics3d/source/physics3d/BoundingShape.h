#pragma once

#include <optional>
#include "gml/vector.h"

namespace physics3d
{
    struct BSphere
    {
        gml::Vec3d center{};
        double radius{1};
    };

    struct BPlane
    {
        gml::Vec3d support{};
        gml::Vec3d original_normal{0.0, 0.0, -1.0};
        gml::Vec3d normal{};
    };

    struct CollisionContact
    {
        gml::Vec3d r_a;
        gml::Vec3d p_a;
        gml::Vec3d r_b;
        gml::Vec3d p_b;
        gml::Vec3d n;
    };

    struct CollisionContactManifold
    {
        // TODO
        CollisionContact contact;
    };

    class CollisionVisitor
    {
    public:
        std::optional<CollisionContactManifold> operator()(const BSphere& a, const BSphere& b);

        std::optional<CollisionContactManifold> operator()(const BSphere& a, const BPlane& b);

        std::optional<CollisionContactManifold> operator()(const BPlane& a, const BSphere& b);

        std::optional<CollisionContactManifold> operator()(const BPlane& a, const BPlane& b);
    };
}

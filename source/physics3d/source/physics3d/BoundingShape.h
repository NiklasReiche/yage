#pragma once

#include <optional>

#include <gml/vector.h>

namespace physics3d
{
    struct ContactManifold;

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

    class CollisionVisitor
    {
    public:
        std::optional<ContactManifold> operator()(const BSphere& a, const BSphere& b);

        std::optional<ContactManifold> operator()(const BSphere& a, const BPlane& b);

        std::optional<ContactManifold> operator()(const BPlane& a, const BSphere& b);

        std::optional<ContactManifold> operator()(const BPlane& a, const BPlane& b);
    };
}

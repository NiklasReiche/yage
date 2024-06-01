#pragma once

#include <optional>
#include <variant>

#include <gml/vector.h>

namespace physics3d
{
    struct ContactManifold;

    /**
     * Represents a non-existent collider.
     */
    struct BNoCollider
    {
    };

    /**
     * Represents a bounding sphere.
     */
    struct BSphere
    {
        gml::Vec3d center{};
        double radius{1};
    };

    /**
     * Represents an infinitely expanding 2D plane.
     */
    struct BPlane
    {
        /**
         * Support point of the plane.
         */
        gml::Vec3d support{};

        /**
         * Original normal without any rotation applied.
         */
        gml::Vec3d original_normal{0.0, 0.0, -1.0};

        /**
         * Actual normal after rotation is taken into account.
         */
        gml::Vec3d normal{};
    };

    using BoundingVolume = std::variant<BNoCollider, BSphere, BPlane>;

    /**
     * Implements collision detection between the various bounding volume types.
     */
    class CollisionVisitor
    {
    public:
        std::optional<ContactManifold> operator()(const BNoCollider& a, const BNoCollider& b);

        std::optional<ContactManifold> operator()(const BNoCollider& a, const BSphere& b);

        std::optional<ContactManifold> operator()(const BNoCollider& a, const BPlane& b);

        std::optional<ContactManifold> operator()(const BSphere& a, const BNoCollider& b);

        std::optional<ContactManifold> operator()(const BSphere& a, const BSphere& b);

        std::optional<ContactManifold> operator()(const BSphere& a, const BPlane& b);

        std::optional<ContactManifold> operator()(const BPlane& a, const BSphere& b);

        std::optional<ContactManifold> operator()(const BPlane& a, const BNoCollider& b);

        std::optional<ContactManifold> operator()(const BPlane& a, const BPlane& b);
    };
}

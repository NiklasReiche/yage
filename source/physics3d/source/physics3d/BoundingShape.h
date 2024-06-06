#pragma once

#include <optional>
#include <variant>

#include <gml/vector.h>

#include "Collision.h"

namespace physics3d
{
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

    struct BOrientedBox
    {
        gml::Vec3d half_size;
        gml::Vec3d center{};
        gml::Quatd orientation{};
        // TODO: we could precompute the original vertices as well as the rotated vertices
        // TODO: normals can also be precomputed
    };

    using BoundingVolume = std::variant<BSphere, BPlane, BOrientedBox>;

    /**
     * Implements collision detection between the various bounding volume types.
     */
    class CollisionVisitor
    {
    public:
        std::optional<ContactManifold> operator()(const BSphere& a, const BSphere& b);

        std::optional<ContactManifold> operator()(const BSphere& a, const BPlane& b);

        std::optional<ContactManifold> operator()(const BSphere&, const BOrientedBox&)
        {
            return {}; // TODO
        }

        std::optional<ContactManifold> operator()(const BPlane& a, const BSphere& b);

        std::optional<ContactManifold> operator()(const BPlane& a, const BPlane& b);

        std::optional<ContactManifold> operator()(const BPlane&, const BOrientedBox&);

        std::optional<ContactManifold> operator()(const BOrientedBox&, const BSphere&)
        {
            return {}; // TODO
        }

        std::optional<ContactManifold> operator()(const BOrientedBox&, const BPlane&);

        std::optional<ContactManifold> operator()(const BOrientedBox& a, const BOrientedBox& b);
    };
}

#pragma once

#include <optional>
#include <variant>

#include <gml/vector.h>

#include "Collision.h"

namespace physics3d
{
    namespace colliders
    {
        /**
         * Represents a bounding sphere.
         */
        struct Sphere
        {
            gml::Vec3d center{};
            double radius{1};
        };

        /**
         * Represents an infinitely expanding 2D plane.
         */
        struct OrientedPlane
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

        /**
         * Represents an oriented bounding box.
         */
        struct OrientedBox
        {
            gml::Vec3d half_size;
            gml::Vec3d center{};
            gml::Quatd orientation{};
            // TODO: we could precompute the original vertices as well as the rotated vertices
            // TODO: normals can also be precomputed
        };
    }

    using Collider = std::variant<colliders::Sphere, colliders::OrientedPlane, colliders::OrientedBox>;

    /**
     * Implements collision detection between the various bounding volume types.
     */
    class CollisionVisitor
    {
    public:
        std::optional<ContactManifold> operator()(const colliders::Sphere& a, const colliders::Sphere& b);

        std::optional<ContactManifold> operator()(const colliders::Sphere& a, const colliders::OrientedPlane& b);

        std::optional<ContactManifold> operator()(const colliders::Sphere&, const colliders::OrientedBox&);

        std::optional<ContactManifold> operator()(const colliders::OrientedPlane& a, const colliders::Sphere& b);

        std::optional<ContactManifold> operator()(const colliders::OrientedPlane& a, const colliders::OrientedPlane& b);

        std::optional<ContactManifold> operator()(const colliders::OrientedPlane&, const colliders::OrientedBox&);

        std::optional<ContactManifold> operator()(const colliders::OrientedBox&, const colliders::Sphere&);

        std::optional<ContactManifold> operator()(const colliders::OrientedBox&, const colliders::OrientedPlane&);

        std::optional<ContactManifold> operator()(const colliders::OrientedBox& a, const colliders::OrientedBox& b);
    };
}

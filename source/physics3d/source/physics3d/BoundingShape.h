#pragma once

#include <optional>
#include <variant>

#include <math/vector.h>

#include "Collision.h"

namespace yage::physics3d
{
    namespace colliders
    {
        /**
         * Represents a bounding sphere.
         */
        struct Sphere
        {
            math::Vec3d center{};
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
            math::Vec3d support{};

            /**
             * Original normal without any rotation applied.
             */
            math::Vec3d original_normal{0.0, 0.0, -1.0};

            /**
             * Actual normal after rotation is taken into account.
             */
            math::Vec3d normal{};
        };

        /**
         * Represents an oriented bounding box.
         */
        struct OrientedBox
        {
            math::Vec3d half_size;
            math::Vec3d center{};
            math::Quatd orientation{};

            std::array<math::Vec3d, 8> oriented_vertices;
            std::array<math::Vec3d, 3> oriented_face_normals;

            void update_computed_values()
            {
                oriented_vertices[0] =
                        orientation * math::Vec3d(-half_size.x(), -half_size.y(), -half_size.z()) + center;
                oriented_vertices[1] =
                        orientation * math::Vec3d(half_size.x(), -half_size.y(), -half_size.z()) + center;
                oriented_vertices[2] =
                        orientation * math::Vec3d(half_size.x(), half_size.y(), -half_size.z()) + center;
                oriented_vertices[3] =
                        orientation * math::Vec3d(-half_size.x(), half_size.y(), -half_size.z()) + center;
                oriented_vertices[4] =
                        orientation * math::Vec3d(-half_size.x(), -half_size.y(), half_size.z()) + center;
                oriented_vertices[5] =
                        orientation * math::Vec3d(half_size.x(), -half_size.y(), half_size.z()) + center;
                oriented_vertices[6] =
                        orientation * math::Vec3d(half_size.x(), half_size.y(), half_size.z()) + center;
                oriented_vertices[7] =
                        orientation * math::Vec3d(-half_size.x(), half_size.y(), half_size.z()) + center;

                oriented_face_normals[0] = normalize(cross(
                        oriented_vertices[1] - oriented_vertices[0],
                        oriented_vertices[3] - oriented_vertices[0]));
                oriented_face_normals[1] = normalize(cross(
                        oriented_vertices[4] - oriented_vertices[0],
                        oriented_vertices[3] - oriented_vertices[0]));
                oriented_face_normals[2] = normalize(cross(
                        oriented_vertices[4] - oriented_vertices[0],
                        oriented_vertices[1] - oriented_vertices[0]));
            }
        };
    }

    using Collider = std::variant<colliders::Sphere, colliders::OrientedPlane, colliders::OrientedBox>;

    /**
     * Implements collision detection between the various bounding volume types.
     */
    class CollisionVisitor
    {
    public:
        std::optional<ContactManifold> operator()(const colliders::Sphere& a, const colliders::Sphere& b) const;

        std::optional<ContactManifold> operator()(const colliders::Sphere& a, const colliders::OrientedPlane& b) const;

        std::optional<ContactManifold> operator()(const colliders::Sphere&, const colliders::OrientedBox&) const;

        std::optional<ContactManifold> operator()(const colliders::OrientedPlane& a, const colliders::Sphere& b) const;

        std::optional<ContactManifold> operator()(const colliders::OrientedPlane& a, const colliders::OrientedPlane& b) const;

        std::optional<ContactManifold> operator()(const colliders::OrientedPlane&, const colliders::OrientedBox&) const;

        std::optional<ContactManifold> operator()(const colliders::OrientedBox&, const colliders::Sphere&) const;

        std::optional<ContactManifold> operator()(const colliders::OrientedBox&, const colliders::OrientedPlane&) const;

        std::optional<ContactManifold> operator()(const colliders::OrientedBox& a, const colliders::OrientedBox& b) const;
    };
}

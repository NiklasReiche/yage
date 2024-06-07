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

            std::array<gml::Vec3d, 8> oriented_vertices;
            std::array<gml::Vec3d, 3> oriented_face_normals;

            void update_computed_values()
            {
                oriented_vertices[0] =
                        orientation * gml::Vec3d(-half_size.x(), -half_size.y(), -half_size.z()) + center;
                oriented_vertices[1] =
                        orientation * gml::Vec3d(half_size.x(), -half_size.y(), -half_size.z()) + center;
                oriented_vertices[2] =
                        orientation * gml::Vec3d(half_size.x(), half_size.y(), -half_size.z()) + center;
                oriented_vertices[3] =
                        orientation * gml::Vec3d(-half_size.x(), half_size.y(), -half_size.z()) + center;
                oriented_vertices[4] =
                        orientation * gml::Vec3d(-half_size.x(), -half_size.y(), half_size.z()) + center;
                oriented_vertices[5] =
                        orientation * gml::Vec3d(half_size.x(), -half_size.y(), half_size.z()) + center;
                oriented_vertices[6] =
                        orientation * gml::Vec3d(half_size.x(), half_size.y(), half_size.z()) + center;
                oriented_vertices[7] =
                        orientation * gml::Vec3d(-half_size.x(), half_size.y(), half_size.z()) + center;

                oriented_face_normals[0] = gml::normalize(gml::cross(
                        oriented_vertices[1] - oriented_vertices[0],
                        oriented_vertices[3] - oriented_vertices[0]));
                oriented_face_normals[1] = gml::normalize(gml::cross(
                        oriented_vertices[4] - oriented_vertices[0],
                        oriented_vertices[3] - oriented_vertices[0]));
                oriented_face_normals[2] = gml::normalize(gml::cross(
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

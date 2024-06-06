#pragma once

#include <span>
#include <optional>

#include <gml/vector.h>

namespace physics3d
{
    namespace geometry
    {
        /**
         * Represents a plane in 3D.
         */
        struct Plane
        {
            /*
             * Any point on the plane.
             */
            gml::Vec3d support;
            /**
             * Plane normal. Whether to consider it pointing inwards or outwards is up to the user.
             */
            gml::Vec3d normal;
        };

        using Rectangle = std::array<gml::Vec3d, 4>;
    }

    /**
     * Runs the Separating Axis Theorem solver.
     * @param vertices_a Vertices of body A in world space.
     * @param vertices_b Vertices of body B in world space.
     * @param normals_a Face normals of body A in world space. Parallel normals are redundant and can be omitted.
     * @param normals_b Face normals of body B in world space. Parallel normals are redundant and can be omitted.
     * @return The minimum translation vector (MTV) between bodies A and B or empty if a separating axis is found.
     * The MTV's magnitude is equal to the overlap between bodies A and B. The MTV points away from A.
     */
    std::optional<gml::Vec3d> sat_3d(std::span<gml::Vec3d> vertices_a,
                                     std::span<gml::Vec3d> vertices_b,
                                     std::span<gml::Vec3d> normals_a,
                                     std::span<gml::Vec3d> normals_b);

    /**
     * Returns the intersection of a non-parallel line and plane. Crashes if the line is parallel to the plane.
     * @param support Any point on the plane.
     * @param normal The plane normal.
     * @param l0 A point on the line.
     * @param l1 Another point on the line.
     * @return The point of intersection.
     */
    gml::Vec3d intersection(const gml::Vec3d& support, const gml::Vec3d& normal,
                            const gml::Vec3d& l0, const gml::Vec3d& l1);

    /**
     * Performs Sutherland-Hodgman clipping of a polygon against a clipping plane.
     * @param clipping_planes Clipping planes with normals pointing inwards.
     * @param polygon Vertices of the polygon to clip, in line-strip order.
     * @return Vertices of the clipped polygon in line-strip order.
     */
    std::vector<gml::Vec3d>
    clip_sutherland_hodgman(std::span<geometry::Plane> clipping_planes, std::span<gml::Vec3d> polygon);

    /**
     * Clips a set of points against a plane by discarding clipped points without intersection replacement.
     * @param clipping_plane Clipping plane with the normal pointing inwards.
     * @param points Points to clip in any order.
     * @return Pairs of remaining points and their (positive) penetration depths w.r.t. to the clipping plane.
     */
    std::vector<std::tuple<gml::Vec3d, double>>
    clip_discard(const geometry::Plane& clipping_plane, std::span<gml::Vec3d> points);

    /**
     * Finds the face of a cube such that the face normal is the most perpendicular to a given vector.
     * @param n The vector to compare against.
     * @param vertices The 8 vertices of the cube following the following ordering convention:
     *    3-------------2
     *   /|            /|
     *  / |           / |
     * 7-------------6  |
     * |  |          |  |      y
     * |  0----------|--1      |
     * | /           | /       o---x
     * |/            |/       /
     * 4-------------5       z
     *
     * @return The most perpendicular face along with its face normal, pointing outwards.
     */
    std::tuple<geometry::Rectangle, gml::Vec3d>
    most_perpendicular_cube_face(const gml::Vec3d& n, std::span<gml::Vec3d> vertices);
}

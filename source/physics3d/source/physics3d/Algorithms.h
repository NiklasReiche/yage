#pragma once

#include <span>
#include <optional>

#include <gml/vector.h>

namespace physics3d
{
    /**
     * Runs the Separating Axis Theorem solver.
     * @param vertices_a Vertices of body A in world space.
     * @param vertices_b Vertices of body B in world space.
     * @param axes Projection axes on which to test for separation
     * @return The minimum translation vector between bodies A and B or empty if a separating axis is found. The length
     * of this vector is equal to the overlap between bodies A and B. The MTV may point from A to B or B to A.
     */
    std::optional<gml::Vec3d> sat_3d(std::span<gml::Vec3d> vertices_a,
                                     std::span<gml::Vec3d> vertices_b,
                                     std::span<gml::Vec3d> axes);
}

#include <vector>
#include "Algorithms.h"

std::optional<gml::Vec3d>
physics3d::sat_3d(std::span<gml::Vec3d> vertices_a, std::span<gml::Vec3d> vertices_b,
                  std::span<gml::Vec3d> normals_a, std::span<gml::Vec3d> normals_b)
{
    std::vector<gml::Vec3d> axes;
    axes.reserve(normals_a.size() + normals_b.size() + normals_a.size() * normals_b.size());
    axes.insert(axes.end(), normals_a.begin(), normals_a.end());
    axes.insert(axes.end(), normals_b.begin(), normals_b.end());
    // for 3D SAT we must additionally check the cross products of each normal from A and each normal from B
    for (const gml::Vec3d& n_a: normals_a) {
        for (const gml::Vec3d& n_b: normals_b) {
            gml::Vec3d cross = gml::cross(n_a, n_b);
            // if the cross product is zero, the normals are parallel, so we can skip this combination
            if (cross != gml::Vec3d(0)) {
                axes.push_back(cross);
            }
        }
    }

    double min_penetration = std::numeric_limits<double>::max();
    gml::Vec3d min_penetration_axis;

    for (const gml::Vec3d& axis: axes) {
        double a_min = std::numeric_limits<double>::max();
        double a_max = std::numeric_limits<double>::min();
        double b_min = std::numeric_limits<double>::max();
        double b_max = std::numeric_limits<double>::min();

        // find the upper and lower bounds of the vertices projected onto the test axis
        for (const gml::Vec3d& vertex: vertices_a) {
            double projection = gml::dot(vertex, axis);
            a_min = std::min(a_min, projection);
            a_max = std::max(a_max, projection);
        }
        for (const gml::Vec3d& vertex: vertices_b) {
            double projection = gml::dot(vertex, axis);
            b_min = std::min(b_min, projection);
            b_max = std::max(b_max, projection);
        }

        // test whether the projected ranges overlap
        if (a_min <= b_min && b_min <= a_max) {
            // a:  a_min |---------------| a_max
            // b:          b_min |----------------| b_max
            double penetration = a_max - b_min;
            if (penetration < min_penetration) {
                min_penetration = penetration;
                min_penetration_axis = axis;
            }
        } else if (b_min <= a_min && a_min <= b_max) {
            // a:         a_min |----------------| a_max
            // b: b_min |--------------| b_max
            double penetration = b_max - a_min;
            if (penetration < min_penetration) {
                min_penetration = penetration;
                min_penetration_axis = -axis;
            }
        } else {
            // no overlap, we found a separating axis
            return {};
        }
    }

    // adjust the length to be equal to the penetration distance
    return {min_penetration * gml::normalize(min_penetration_axis)};
}

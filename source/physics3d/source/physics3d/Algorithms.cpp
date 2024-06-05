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

gml::Vec3d
physics3d::intersection(const gml::Vec3d& support, const gml::Vec3d& normal, const gml::Vec3d& l0, const gml::Vec3d& l1)
{
    gml::Vec3d line_direction = gml::normalize(l1 - l0);
    double d = gml::dot(support - l0, normal) / gml::dot(line_direction, normal);
    return l0 + (line_direction * d);
}

std::vector<gml::Vec3d>
physics3d::clip_sutherland_hodgman(std::span<geometry::Plane> clipping_planes, std::span<gml::Vec3d> polygon)
{
    std::vector<gml::Vec3d> output;
    output.insert(output.end(), polygon.begin(), polygon.end());
    std::vector<gml::Vec3d> input;
    input.reserve(output.size());

    for (const auto& [support, normal] : clipping_planes) {
        input.clear();
        input.insert(input.end(), output.begin(), output.end());
        output.clear();
        for (std::size_t i = 0; i < input.size(); ++i) {
            gml::Vec3d current = input[i];
            gml::Vec3d previous = input[(i - 1) % 4];
            if (gml::dot(current - support, normal) >= 0) { // current is inside
                if (gml::dot(previous - support, normal) < 0) { // previous is outside
                    output.push_back(intersection(support, normal, previous, current));
                }
                output.push_back(current);
            } else if (gml::dot(previous - support, normal) >= 0) { // current is outside && previous is inside
                output.push_back(intersection(support, normal, previous, current));
            }
        }
    }

    return output;
}

std::vector<std::tuple<gml::Vec3d, double>>
physics3d::clip_discard(const geometry::Plane& clipping_plane, std::span<gml::Vec3d> points)
{
    std::vector<std::tuple<gml::Vec3d, double>> result;
    result.reserve(points.size());
    for (gml::Vec3d vertex: points) {
        double dist = gml::dot(vertex - clipping_plane.support, clipping_plane.normal);
        if (dist >= 0) { // point is inside
            result.emplace_back(vertex, dist);
        }
    }
    return result;
}

#include <vector>
#include "Algorithms.h"

namespace yage::physics3d
{
    std::optional<math::Vec3d>
    sat_3d(std::span<const math::Vec3d> vertices_a, std::span<const math::Vec3d> vertices_b,
                      std::span<const math::Vec3d> normals_a, std::span<const math::Vec3d> normals_b)
    {
        std::vector<math::Vec3d> axes;
        axes.reserve(normals_a.size() + normals_b.size() + normals_a.size() * normals_b.size());
        axes.insert(axes.end(), normals_a.begin(), normals_a.end());
        axes.insert(axes.end(), normals_b.begin(), normals_b.end());
        // for 3D SAT we must additionally check the cross products of each normal from A and each normal from B
        for (const math::Vec3d& n_a: normals_a) {
            for (const math::Vec3d& n_b: normals_b) {
                // if the cross product is zero, the normals are parallel, so we can skip this combination
                if (math::Vec3d cross = math::cross(n_a, n_b);
                    cross != math::Vec3d(0)) {
                    axes.push_back(math::normalize(cross));
                }
            }
        }

        double min_penetration = std::numeric_limits<double>::max();
        math::Vec3d min_penetration_axis;

        for (const math::Vec3d& axis: axes) {
            double a_min = std::numeric_limits<double>::max();
            double a_max = std::numeric_limits<double>::min();
            double b_min = std::numeric_limits<double>::max();
            double b_max = std::numeric_limits<double>::min();

            // find the upper and lower bounds of the vertices projected onto the test axis
            for (const math::Vec3d& vertex: vertices_a) {
                double projection = dot(vertex, axis);
                a_min = std::min(a_min, projection);
                a_max = std::max(a_max, projection);
            }
            for (const math::Vec3d& vertex: vertices_b) {
                double projection = dot(vertex, axis);
                b_min = std::min(b_min, projection);
                b_max = std::max(b_max, projection);
            }

            // test whether the projected ranges overlap
            if (a_min <= b_min && b_min <= a_max) {
                // a:  a_min |---------------| a_max
                // b:          b_min |----~~~~~~~
                const double penetration = std::min(a_max, b_max) - b_min;
                if (penetration < min_penetration) {
                    min_penetration = penetration;
                    min_penetration_axis = axis;
                }
            } else if (b_min <= a_min && a_min <= b_max) {
                // a:         a_min |---~~~~~~~~~
                // b: b_min |--------------| b_max
                const double penetration = std::min(b_max, a_max) - a_min;
                if (penetration < min_penetration) {
                    min_penetration = penetration;
                    min_penetration_axis = -axis; // invert axis so it points away from A
                }
            } else {
                // no overlap, we found a separating axis
                return {};
            }
        }

        // adjust the length to be equal to the penetration distance
        return {min_penetration * normalize(min_penetration_axis)};
    }

    math::Vec3d
    intersection(const math::Vec3d& support, const math::Vec3d& normal, const math::Vec3d& l0,
                            const math::Vec3d& l1)
    {
        const math::Vec3d line_direction = normalize(l1 - l0);
        const double d = dot(support - l0, normal) / dot(line_direction, normal);
        return l0 + line_direction * d;
    }

    std::vector<math::Vec3d>
    clip_sutherland_hodgman(std::span<geometry::Plane> clipping_planes, std::span<const math::Vec3d> polygon)
    {
        std::vector<math::Vec3d> output;
        output.insert(output.end(), polygon.begin(), polygon.end());
        std::vector<math::Vec3d> input;
        input.reserve(output.size());

        for (const auto& [support, normal]: clipping_planes) {
            input.clear();
            input.insert(input.end(), output.begin(), output.end());
            output.clear();
            for (std::size_t i = 0; i < input.size(); ++i) {
                math::Vec3d current = input[i];
                math::Vec3d previous = input[(i - 1) % input.size()];
                if (dot(current - support, normal) >= 0) {
                    // current is inside
                    if (dot(previous - support, normal) < 0) {
                        // previous is outside
                        output.push_back(intersection(support, normal, previous, current));
                    }
                    output.push_back(current);
                } else if (dot(previous - support, normal) >= 0) {
                    // current is outside && previous is inside
                    output.push_back(intersection(support, normal, previous, current));
                }
            }
        }

        return output;
    }

    std::vector<std::tuple<math::Vec3d, double> >
    clip_discard(const geometry::Plane& clipping_plane, std::span<const math::Vec3d> points)
    {
        std::vector<std::tuple<math::Vec3d, double> > result;
        result.reserve(points.size());
        for (math::Vec3d vertex: points) {
            double dist = dot(vertex - clipping_plane.support, clipping_plane.normal);
            if (dist >= 0) {
                // point is inside
                result.emplace_back(vertex, dist);
            }
        }
        return result;
    }

    std::tuple<geometry::Rectangle, math::Vec3d>
    most_perpendicular_cube_face(const math::Vec3d& n, const std::span<const math::Vec3d> vertices)
    {
        // encodes the 3 adjacent faces (4 vertex indices and one normal index) for each vertex
        // note that the normal is expressed by the vertex opposite the corner in the (negative) normal direction
        constexpr std::array encoding{
            // vertex 0:
            std::array{
                0, 4, 7, 3, 1, // x-face (vertex, vertex, vertex, vertex, normal)
                0, 1, 5, 4, 3, // y-face
                0, 1, 2, 3, 4, // z-face
            },
            // vertex 1:
            std::array{
                1, 5, 6, 2, 0,
                1, 5, 4, 0, 2,
                1, 2, 3, 0, 5,
            },
            // vertex 2:
            std::array{
                2, 1, 5, 6, 3,
                2, 6, 7, 3, 1,
                2, 1, 0, 3, 6,
            },
            // vertex 3:
            std::array{
                3, 0, 4, 7, 2,
                3, 2, 6, 7, 0,
                3, 2, 1, 0, 7,
            },
            // vertex 4:
            std::array{

                4, 0, 3, 7, 5,
                4, 0, 1, 5, 7,
                4, 5, 6, 7, 0,
            },
            // vertex 5:
            std::array{
                5, 1, 2, 6, 4,
                5, 1, 0, 4, 6,
                5, 6, 7, 4, 1,
            },
            // vertex 6:
            std::array{
                6, 5, 1, 2, 7,
                6, 2, 3, 7, 5,
                6, 5, 4, 7, 2,
            },
            // vertex 7:
            std::array{
                7, 3, 0, 4, 6,
                7, 3, 2, 6, 4,
                7, 6, 4, 5, 3,
            },
        };

        // find the vertex farthest along the target vector n
        std::size_t max_i = 0;
        double max = dot(vertices[0], n);
        for (std::size_t i = 1; i < vertices.size(); ++i) {
            const double proj = dot(vertices[i], n);
            if (proj > max) {
                max = proj;
                max_i = i;
            }
        }

        // find the faces that the farthest vertex (v0) is part of
        const math::Vec3d v0 = vertices[max_i];
        geometry::Rectangle face_x = {
            vertices[encoding[max_i][0]], vertices[encoding[max_i][1]],
            vertices[encoding[max_i][2]], vertices[encoding[max_i][3]]
        };
        math::Vec3d normal_x = normalize(v0 - vertices[encoding[max_i][4]]);

        geometry::Rectangle face_y = {
            vertices[encoding[max_i][5]], vertices[encoding[max_i][6]],
            vertices[encoding[max_i][7]], vertices[encoding[max_i][8]]
        };
        math::Vec3d normal_y = normalize(v0 - vertices[encoding[max_i][9]]);

        geometry::Rectangle face_z = {
            vertices[encoding[max_i][10]], vertices[encoding[max_i][11]],
            vertices[encoding[max_i][12]], vertices[encoding[max_i][13]]
        };
        math::Vec3d normal_z = normalize(v0 - vertices[encoding[max_i][14]]);

        // find the face that is most perpendicular to the target vector n
        if (dot(normal_x, n) > dot(normal_y, n) &&
            dot(normal_x, n) > dot(normal_z, n)) {
            return {face_x, normal_x};
        }
        if (dot(normal_y, n) > dot(normal_x, n) &&
            dot(normal_y, n) > dot(normal_z, n)) {
            return {face_y, normal_y};
        }
        return {face_z, normal_z};
    }
}

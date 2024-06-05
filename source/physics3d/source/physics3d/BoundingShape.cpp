#include <algorithm>
#include <map>
#include "BoundingShape.h"
#include "Collision.h"
#include "Algorithms.h"

namespace physics3d
{
    /**
     * @return Positive penetration depth along the contact normal n.
     */
    double penetration_distance(const gml::Vec3d& p_a, const gml::Vec3d& p_b, gml::Vec3d n)
    {
        // since p_a lies within b and p_b lies within a and n is from a to b, we have to project onto the negated
        // normal to get a positive penetration depth
        return gml::dot(p_b - p_a, -n);
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BNoCollider&, const BNoCollider&)
    {
        return {}; // no collision with non-existent collider
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BNoCollider&, const BSphere&)
    {
        return {}; // no collision with non-existent collider
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BNoCollider&, const BPlane&)
    {
        return {}; // no collision with non-existent collider
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BSphere&, const BNoCollider&)
    {
        return {}; // no collision with non-existent collider
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BSphere& a, const BSphere& b)
    {
        auto ab = b.center - a.center;
        if (gml::sqrLength(ab) > (a.radius + b.radius) * (a.radius + b.radius)) {
            return {};
        }

        ContactManifold manifold;
        manifold.normal = gml::normalize(b.center - a.center);

        ContactPoint contact;
        contact.r_a = manifold.normal * a.radius;
        contact.p_a = a.center + contact.r_a;
        contact.r_b = -manifold.normal * a.radius;
        contact.p_b = b.center + contact.r_b;
        contact.depth = penetration_distance(contact.p_a, contact.p_b, manifold.normal);

        manifold.contacts.push_back(contact);
        return {manifold};
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BSphere& a, const BPlane& b)
    {
        // dist is positive if the circle collides on the outer side and negative otherwise (b.normal points outward)
        auto dist = gml::dot(a.center - b.support,
                             b.normal); // projection does not need denominator, since normal is unit length
        auto abs_dist = std::abs(dist);
        if (abs_dist > a.radius) {
            return {};
        }

        ContactManifold manifold;
        manifold.normal = dist > 0 ? -b.normal : b.normal;

        ContactPoint contact;
        contact.r_a = manifold.normal * a.radius;
        contact.p_a = a.center + contact.r_a;
        contact.p_b = a.center + manifold.normal * abs_dist;
        contact.r_b = contact.p_b - b.support;
        contact.depth = penetration_distance(contact.p_a, contact.p_b, manifold.normal);

        manifold.contacts.push_back(contact);
        return {manifold};
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BPlane&, const BNoCollider&)
    {
        return {}; // no collision with non-existent collider
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BPlane& a, const BSphere& b)
    {
        // dist is positive if the circle collides on the outer side and negative otherwise (b.normal points outward)
        auto dist = gml::dot(b.center - a.support,
                             a.normal); // projection does not need denominator, since normal is unit length
        auto abs_dist = std::abs(dist);
        if (abs_dist > b.radius) {
            return {};
        }

        ContactManifold manifold;
        manifold.normal = dist > 0 ? a.normal : -a.normal;

        ContactPoint contact;
        contact.r_b = -manifold.normal * b.radius;
        contact.p_b = b.center + contact.r_b;
        contact.p_a = b.center - manifold.normal * abs_dist;
        contact.r_a = contact.p_a - a.support;
        contact.depth = penetration_distance(contact.p_a, contact.p_b, manifold.normal);

        manifold.contacts.push_back(contact);
        return {manifold};
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BPlane&, const BPlane&)
    {
        return {}; // planes should probably not collide with themselves, since they are infinite
    }

    using Face = std::array<gml::Vec3d, 4>;

    std::pair<Face, gml::Vec3d> best_face(const gml::Vec3d& n, const std::vector<gml::Vec3d>& vertices)
    {
        /*
         *    3-------------2
         *   /|            /|
         *  / |           / |
         * 7-------------6  |
         * |  |          |  |      y
         * |  0----------|--1      |
         * | /           | /       o---x
         * |/            |/       /
         * 4-------------5       z
         */

        constexpr const std::array<std::array<int, 15>, 8> encoding{
                // vertex 0:
                std::array<int, 15>{
                        0, 4, 7, 3, 1, // x-face (vertex, vertex, vertex, vertex, normal)
                        0, 1, 5, 4, 3, // y-face
                        0, 1, 2, 3, 4, // z-face
                },
                // vertex 1:
                std::array<int, 15>{
                        1, 5, 6, 2, 0,
                        1, 5, 4, 0, 2,
                        1, 2, 3, 0, 5,
                },
                // vertex 2:
                std::array<int, 15>{
                        2, 1, 5, 6, 3,
                        2, 6, 7, 3, 1,
                        2, 1, 0, 3, 6,
                },
                // vertex 3:
                std::array<int, 15>{
                        3, 0, 4, 7, 2,
                        3, 2, 6, 7, 0,
                        3, 2, 1, 0, 7,
                },
                // vertex 4:
                std::array<int, 15>{
                        4, 0, 3, 7, 5,
                        4, 0, 1, 5, 7,
                        4, 5, 6, 7, 0,
                },
                // vertex 5:
                std::array<int, 15>{
                        5, 1, 2, 6, 4,
                        5, 1, 0, 4, 6,
                        5, 6, 7, 4, 1,
                },
                // vertex 6:
                std::array<int, 15>{
                        6, 5, 1, 2, 7,
                        6, 2, 3, 7, 5,
                        6, 5, 4, 7, 2,
                },
                // vertex 7:
                std::array<int, 15>{
                        7, 3, 0, 4, 6,
                        7, 3, 2, 6, 4,
                        7, 6, 4, 5, 3,
                },
        };

        // find the vertex farthest along the collision normal
        std::size_t max_i = 0;
        double max = gml::dot(vertices[0], n);
        for (std::size_t i = 0; i < vertices.size(); ++i) {
            double proj = gml::dot(vertices[i], n);
            if (proj > max) {
                max = proj;
                max_i = i;
            }
        }

        // find the faces that the farthest vertex is part of
        auto v0 = vertices[max_i];
        Face fx = {vertices[encoding[max_i][0]], vertices[encoding[max_i][1]],
                   vertices[encoding[max_i][2]], vertices[encoding[max_i][3]]};
        gml::Vec3d nx = gml::normalize(v0 - vertices[encoding[max_i][4]]);
        Face fy = {vertices[encoding[max_i][5]], vertices[encoding[max_i][6]],
                   vertices[encoding[max_i][7]], vertices[encoding[max_i][8]]};
        gml::Vec3d ny = gml::normalize(v0 - vertices[encoding[max_i][9]]);
        Face fz = {vertices[encoding[max_i][10]], vertices[encoding[max_i][11]],
                   vertices[encoding[max_i][12]], vertices[encoding[max_i][13]]};
        gml::Vec3d nz = gml::normalize(v0 - vertices[encoding[max_i][14]]);

        // find the face that is most perpendicular to the collision normal
        if (gml::dot(nx, n) > gml::dot(ny, n) &&
            gml::dot(nx, n) > gml::dot(nz, n)) {
            return {fx, nx};
        } else if (gml::dot(ny, n) > gml::dot(nx, n) &&
                   gml::dot(ny, n) > gml::dot(nz, n)) {
            return {fy, ny};
        } else {
            return {fz, nz};
        }
    }

    gml::Vec3d intersection(const gml::Vec3d& support, const gml::Vec3d& n, const gml::Vec3d& v0, const gml::Vec3d& v1)
    {
        gml::Vec3d line_direction = gml::normalize(v1 - v0);
        double d = gml::dot(support - v0, n) / gml::dot(line_direction, n);
        return v0 + (line_direction * d);
    }

    std::vector<gml::Vec3d> clip(const gml::Vec3d& support, const gml::Vec3d& normal, const std::vector<gml::Vec3d>& input)
    {
        std::vector<gml::Vec3d> output;
        output.reserve(input.size());
        for (std::size_t i = 0; i < input.size(); ++i) {
            gml::Vec3d current = input[i];
            gml::Vec3d previous = input[(i-1) % 4];
            if (gml::dot(current - support, normal) >= 0) { // current inside
                if (gml::dot(previous - support, normal) < 0) { // previous outside
                    output.push_back(intersection(support, normal, previous, current));
                }
                output.push_back(current);
            } else if (gml::dot(previous - support, normal) >= 0) { // current outside && previous inside
                output.push_back(intersection(support, normal, previous, current));
            }
        }
        return output;
    }

    std::vector<std::pair<gml::Vec3d, double>> clip_discard(const gml::Vec3d& support, const gml::Vec3d& normal, const std::vector<gml::Vec3d>& input)
    {
        std::vector<std::pair<gml::Vec3d, double>> result;
        result.reserve(input.size());
        for (gml::Vec3d vertex : input) {
            double dist = gml::dot(vertex - support, normal);
            if (dist < 0) { // outside
                result.emplace_back(vertex, -dist);
            }
        }
        return result;
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const BOrientedBox& a, const BOrientedBox& b)
    {
        /*
         *    3-------------2
         *   /|            /|
         *  / |           / |
         * 7-------------6  |
         * |  |          |  |      y
         * |  0----------|--1      |
         * | /           | /       o---x
         * |/            |/       /
         * 4-------------5       z
         */

        std::vector<gml::Vec3d> vertices_a;
        vertices_a.emplace_back(-a.half_size.x(), -a.half_size.y(), -a.half_size.z());
        vertices_a.emplace_back(a.half_size.x(), -a.half_size.y(), -a.half_size.z());
        vertices_a.emplace_back(a.half_size.x(), a.half_size.y(), -a.half_size.z());
        vertices_a.emplace_back(-a.half_size.x(), a.half_size.y(), -a.half_size.z());
        vertices_a.emplace_back(-a.half_size.x(), -a.half_size.y(), a.half_size.z());
        vertices_a.emplace_back(a.half_size.x(), -a.half_size.y(), a.half_size.z());
        vertices_a.emplace_back(a.half_size.x(), a.half_size.y(), a.half_size.z());
        vertices_a.emplace_back(-a.half_size.x(), a.half_size.y(), a.half_size.z());
        for (auto& vertex: vertices_a) {
            vertex = a.orientation * vertex + a.center;
        }

        std::vector<gml::Vec3d> normals_a;
        normals_a.push_back(gml::normalize(gml::cross(vertices_a[1] - vertices_a[0], vertices_a[3] - vertices_a[0])));
        normals_a.push_back(gml::normalize(gml::cross(vertices_a[4] - vertices_a[0], vertices_a[3] - vertices_a[0])));
        normals_a.push_back(gml::normalize(gml::cross(vertices_a[4] - vertices_a[0], vertices_a[1] - vertices_a[0])));

        std::vector<gml::Vec3d> vertices_b;
        vertices_b.emplace_back(-b.half_size.x(), -b.half_size.y(), -b.half_size.z());
        vertices_b.emplace_back(b.half_size.x(), -b.half_size.y(), -b.half_size.z());
        vertices_b.emplace_back(b.half_size.x(), b.half_size.y(), -b.half_size.z());
        vertices_b.emplace_back(-b.half_size.x(), b.half_size.y(), -b.half_size.z());
        vertices_b.emplace_back(-b.half_size.x(), -b.half_size.y(), b.half_size.z());
        vertices_b.emplace_back(b.half_size.x(), -b.half_size.y(), b.half_size.z());
        vertices_b.emplace_back(b.half_size.x(), b.half_size.y(), b.half_size.z());
        vertices_b.emplace_back(-b.half_size.x(), b.half_size.y(), b.half_size.z());
        for (auto& vertex: vertices_b) {
            vertex = b.orientation * vertex + b.center;
        }

        std::vector<gml::Vec3d> normals_b;
        normals_b.push_back(gml::normalize(gml::cross(vertices_b[1] - vertices_b[0], vertices_b[3] - vertices_b[0])));
        normals_b.push_back(gml::normalize(gml::cross(vertices_b[4] - vertices_b[0], vertices_b[3] - vertices_b[0])));
        normals_b.push_back(gml::normalize(gml::cross(vertices_b[4] - vertices_b[0], vertices_b[1] - vertices_b[0])));

        // get the minimum translation vector with the SAT
        std::optional<gml::Vec3d> maybe_mtv = sat_3d(vertices_a, vertices_b, normals_a, normals_b);
        if (!maybe_mtv.has_value()) {
            return {};
        }

        ContactManifold manifold;
        // make sure that the collision normal points away from A
        if (gml::dot(maybe_mtv.value(), b.center - a.center) >= 0) {
            manifold.normal = gml::normalize(maybe_mtv.value());
        } else {
            manifold.normal = -gml::normalize(maybe_mtv.value());
        }


        auto [f_a, n_a] = best_face(manifold.normal, vertices_a);
        auto [f_b, n_b] = best_face(-manifold.normal, vertices_b);

        Face reference, incident;
        bool flipped = false;
        if (gml::dot(n_a, manifold.normal) > gml::dot(n_b, -manifold.normal)) {
            reference = f_a;
            incident = f_b;
        } else {
            reference = f_b;
            incident = f_a;
            flipped = true;
        }
        std::vector<gml::Vec3d> contact_points = {incident[0], incident[1], incident[2], incident[3]};
        // Sutherland-Hodgman clip against he 4 adjacent faces
        contact_points = clip(reference[0], gml::normalize(reference[1] - reference[0]), contact_points);
        contact_points = clip(reference[1], gml::normalize(reference[2] - reference[1]), contact_points);
        contact_points = clip(reference[2], gml::normalize(reference[3] - reference[2]), contact_points);
        contact_points = clip(reference[3], gml::normalize(reference[0] - reference[3]), contact_points);
        // clip and discard against reference face
        auto contacts = clip_discard(reference[0], flipped ? -manifold.normal : manifold.normal, contact_points);

        for (auto& [p, d] : contacts) {
            ContactPoint contact;
            contact.depth = d;
            contact.p_a = flipped ? p : p + -manifold.normal * d;
            contact.p_b = flipped ? p + -manifold.normal * d : p;
            contact.r_a = contact.p_a - a.center;
            contact.r_b = contact.p_b - b.center;

            manifold.contacts.push_back(contact);
        }

        return {manifold};
    }
}
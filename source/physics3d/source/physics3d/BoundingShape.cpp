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

        // TODO: these can be precomputed
        std::vector<gml::Vec3d> vertices_a{
                gml::Vec3d(-a.half_size.x(), -a.half_size.y(), -a.half_size.z()),
                gml::Vec3d(a.half_size.x(), -a.half_size.y(), -a.half_size.z()),
                gml::Vec3d(a.half_size.x(), a.half_size.y(), -a.half_size.z()),
                gml::Vec3d(-a.half_size.x(), a.half_size.y(), -a.half_size.z()),
                gml::Vec3d(-a.half_size.x(), -a.half_size.y(), a.half_size.z()),
                gml::Vec3d(a.half_size.x(), -a.half_size.y(), a.half_size.z()),
                gml::Vec3d(a.half_size.x(), a.half_size.y(), a.half_size.z()),
                gml::Vec3d(-a.half_size.x(), a.half_size.y(), a.half_size.z()),
        };
        std::vector<gml::Vec3d> vertices_b{
                gml::Vec3d(-b.half_size.x(), -b.half_size.y(), -b.half_size.z()),
                gml::Vec3d(b.half_size.x(), -b.half_size.y(), -b.half_size.z()),
                gml::Vec3d(b.half_size.x(), b.half_size.y(), -b.half_size.z()),
                gml::Vec3d(-b.half_size.x(), b.half_size.y(), -b.half_size.z()),
                gml::Vec3d(-b.half_size.x(), -b.half_size.y(), b.half_size.z()),
                gml::Vec3d(b.half_size.x(), -b.half_size.y(), b.half_size.z()),
                gml::Vec3d(b.half_size.x(), b.half_size.y(), b.half_size.z()),
                gml::Vec3d(-b.half_size.x(), b.half_size.y(), b.half_size.z()),
        };

        for (gml::Vec3d& vertex: vertices_a) {
            vertex = a.orientation * vertex + a.center;
        }
        std::vector<gml::Vec3d> normals_a{
                gml::normalize(gml::cross(vertices_a[1] - vertices_a[0], vertices_a[3] - vertices_a[0])),
                gml::normalize(gml::cross(vertices_a[4] - vertices_a[0], vertices_a[3] - vertices_a[0])),
                gml::normalize(gml::cross(vertices_a[4] - vertices_a[0], vertices_a[1] - vertices_a[0]))
        };

        for (gml::Vec3d& vertex: vertices_b) {
            vertex = b.orientation * vertex + b.center;
        }
        std::vector<gml::Vec3d> normals_b{
                gml::normalize(gml::cross(vertices_b[1] - vertices_b[0], vertices_b[3] - vertices_b[0])),
                gml::normalize(gml::cross(vertices_b[4] - vertices_b[0], vertices_b[3] - vertices_b[0])),
                gml::normalize(gml::cross(vertices_b[4] - vertices_b[0], vertices_b[1] - vertices_b[0])),
        };

        // get the minimum translation vector with the SAT
        std::optional<gml::Vec3d> maybe_mtv = sat_3d(vertices_a, vertices_b, normals_a, normals_b);
        if (!maybe_mtv.has_value()) {
            return {};
        }

        ContactManifold manifold;
        manifold.normal = gml::normalize(maybe_mtv.value());

        const auto [face_a, face_a_normal] =
                most_perpendicular_cube_face(manifold.normal, vertices_a);
        const auto [face_b, face_b_normal] =
                most_perpendicular_cube_face(-manifold.normal, vertices_b);

        geometry::Rectangle reference, incident;
        bool flipped = false;
        // choose the more perpendicular face as the reference face
        if (gml::dot(face_a_normal, manifold.normal) > gml::dot(face_b_normal, -manifold.normal)) {
            reference = face_a;
            incident = face_b;
        } else {
            reference = face_b;
            incident = face_a;
            flipped = true;
        }
        std::vector<gml::Vec3d> contact_points = {incident[0], incident[1], incident[2], incident[3]};
        // Sutherland-Hodgman clip against the 4 adjacent faces
        std::array<geometry::Plane, 4> clipping_planes = {
                geometry::Plane{.support = reference[0], .normal = gml::normalize(reference[1] - reference[0])},
                geometry::Plane{.support = reference[1], .normal = gml::normalize(reference[2] - reference[1])},
                geometry::Plane{.support = reference[2], .normal = gml::normalize(reference[3] - reference[2])},
                geometry::Plane{.support = reference[3], .normal = gml::normalize(reference[0] - reference[3])},
        };
        contact_points = clip_sutherland_hodgman(clipping_planes, contact_points);
        // clip and discard against reference face
        std::vector<std::tuple<gml::Vec3d, double>> contacts_with_depth = clip_discard(
                geometry::Plane{
                        .support = reference[0],
                        .normal = flipped ? manifold.normal : -manifold.normal
                },
                contact_points);

        for (const auto& [point, depth]: contacts_with_depth) {
            ContactPoint contact;
            contact.depth = depth;
            contact.p_a = flipped ? point : point + manifold.normal * depth;
            contact.p_b = flipped ? point - manifold.normal * depth : point;
            contact.r_a = contact.p_a - a.center;
            contact.r_b = contact.p_b - b.center;

            manifold.contacts.push_back(contact);
        }

        return {manifold};
    }
}
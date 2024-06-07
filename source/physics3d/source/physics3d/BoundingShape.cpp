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

    std::optional<ContactManifold> CollisionVisitor::operator()(const colliders::Sphere& a, const colliders::Sphere& b)
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

    std::optional<ContactManifold> CollisionVisitor::operator()(const colliders::Sphere& a, const colliders::OrientedPlane& b)
    {
        // The direction of the collision is determined by which side of the plane has more overlap with the sphere,
        // which means that if the sphere's center overshoots the plane, the direction is the wrong way around.
        // This can be solved by continuous collision detection or by incorporating the relative velocity here.

        // dist is positive if the circle collides on the outer side and negative otherwise (b.normal points outward)
        auto dist = gml::dot(a.center - b.support, b.normal);
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

    std::optional<ContactManifold>
    CollisionVisitor::operator()(const colliders::Sphere& a, const colliders::OrientedBox& b)
    {
        // transform the sphere's center into the local space of the box
        gml::Vec3d center = gml::conjugate(b.orientation) * (a.center - b.center);
        // find the closest point on the box to the sphere's center in the box's local space
        gml::Vec3d point;
        point.x() = std::max(-b.half_size.x(), std::min(b.half_size.x(), center.x()));
        point.y() = std::max(-b.half_size.y(), std::min(b.half_size.y(), center.y()));
        point.z() = std::max(-b.half_size.z(), std::min(b.half_size.z(), center.z()));
        // transform point to world space
        point = b.orientation * point + b.center;

        gml::Vec3d offset = point - a.center;
        double offset_length = gml::length(offset);
        if (offset_length > a.radius) {
            return {};
        }

        ContactManifold manifold;
        manifold.normal = gml::normalize(offset);

        ContactPoint contact;
        contact.depth = a.radius - offset_length;
        contact.p_a = a.center + a.radius * manifold.normal;
        contact.p_b = point;
        contact.r_a = contact.p_a - a.center;
        contact.r_b = contact.p_b - b.center;

        manifold.contacts.push_back(contact);
        return {manifold};
    }

    std::optional<ContactManifold> CollisionVisitor::operator()(const colliders::OrientedPlane& a, const colliders::Sphere& b)
    {
        // The direction of the collision is determined by which side of the plane has more overlap with the sphere,
        // which means that if the sphere's center overshoots the plane, the direction is the wrong way around.
        // This can be solved by continuous collision detection or by incorporating the relative velocity here.

        // dist is positive if the circle collides on the outer side and negative otherwise (b.normal points outward)
        auto dist = gml::dot(b.center - a.support, a.normal);
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

    std::optional<ContactManifold> CollisionVisitor::operator()(const colliders::OrientedPlane&, const colliders::OrientedPlane&)
    {
        return {}; // planes should probably not collide with themselves, since they are infinite
    }

    std::optional<ContactManifold>
    CollisionVisitor::operator()(const colliders::OrientedPlane& a, const colliders::OrientedBox& b)
    {
        // The direction of the collision is determined by which side of the plane has more overlap with the box,
        // which means that if the sphere's center overshoots the plane, the direction is the wrong way around.
        // This can be solved by continuous collision detection or by incorporating the relative velocity here.

        /*             ___      ^                          ^
         *            | b |     |                  ___     |
         *      --a---|___|--------         --a---| b |--------
         *                                        |___|
         */

        // dist is positive if the box collides on the outer side and negative otherwise (b.normal points outward)
        auto dist = gml::dot(b.center - a.support, a.normal);
        ContactManifold manifold;
        manifold.normal = dist > 0 ? a.normal : -a.normal;

        std::vector<std::tuple<gml::Vec3d, double>> contacts_with_depth =
                clip_discard(geometry::Plane{.support = a.support, .normal = -manifold.normal}, b.oriented_vertices);

        if (contacts_with_depth.empty())
            return {};

        for (const auto& [point, depth]: contacts_with_depth) {
            ContactPoint contact;
            contact.depth = depth;
            contact.p_a = point + manifold.normal * depth;
            contact.p_b = point;
            contact.r_a = contact.p_a - a.support;
            contact.r_b = contact.p_b - b.center;

            manifold.contacts.push_back(contact);
        }

        return manifold;
    }

    std::optional<ContactManifold>
    CollisionVisitor::operator()(const colliders::OrientedBox& a, const colliders::Sphere& b)
    {
        // transform the sphere's center into the local space of the box
        gml::Vec3d center = gml::conjugate(a.orientation) * (b.center - a.center);
        // find the closest point on the box to the sphere's center in the box's local space
        gml::Vec3d point;
        point.x() = std::max(-a.half_size.x(), std::min(a.half_size.x(), center.x()));
        point.y() = std::max(-a.half_size.y(), std::min(a.half_size.y(), center.y()));
        point.z() = std::max(-a.half_size.z(), std::min(a.half_size.z(), center.z()));
        // transform point to world space
        point = a.orientation * point + a.center;

        gml::Vec3d offset = point - b.center;
        double offset_length = gml::length(offset);
        if (offset_length > b.radius) {
            return {};
        }

        ContactManifold manifold;
        manifold.normal = -gml::normalize(offset);

        ContactPoint contact;
        contact.depth = b.radius - offset_length;
        contact.p_a = point;
        contact.p_b = b.center - b.radius * manifold.normal;
        contact.r_a = contact.p_a - a.center;
        contact.r_b = contact.p_b - b.center;

        manifold.contacts.push_back(contact);
        return {manifold};
    }

    std::optional<ContactManifold>
    CollisionVisitor::operator()(const colliders::OrientedBox& a, const colliders::OrientedPlane& b)
    {
        // The direction of the collision is determined by which side of the plane has more overlap with the box,
        // which means that if the sphere's center overshoots the plane, the direction is the wrong way around.
        // This can be solved by continuous collision detection or by incorporating the relative velocity here.

        /*             ___      ^                          ^
         *            | a |     |                  ___     |
         *      --b---|___|--------         --b---| a |--------
         *                                        |___|
         */

        // dist is positive if the circle collides on the outer side and negative otherwise (b.normal points outward)
        auto dist = gml::dot(a.center - b.support, b.normal);
        ContactManifold manifold;
        manifold.normal = dist > 0 ? -b.normal : b.normal;

        std::vector<std::tuple<gml::Vec3d, double>> contacts_with_depth =
                clip_discard(geometry::Plane{.support = b.support, .normal = manifold.normal}, a.oriented_vertices);

        if (contacts_with_depth.empty())
            return {};

        for (const auto& [point, depth]: contacts_with_depth) {
            ContactPoint contact;
            contact.depth = depth;
            contact.p_a = point;
            contact.p_b = point - manifold.normal * depth;
            contact.r_a = contact.p_a - a.center;
            contact.r_b = contact.p_b - b.support;

            manifold.contacts.push_back(contact);
        }

        return manifold;
    }

    std::optional<ContactManifold>
    CollisionVisitor::operator()(const colliders::OrientedBox& a, const colliders::OrientedBox& b)
    {
        /* Encoding convention for box vertices:
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

        // get the minimum translation vector with the SAT
        std::optional<gml::Vec3d> maybe_mtv = sat_3d(a.oriented_vertices, b.oriented_vertices,
                                                     a.oriented_face_normals, b.oriented_face_normals);
        if (!maybe_mtv.has_value()) {
            return {};
        }

        ContactManifold manifold;
        manifold.normal = gml::normalize(maybe_mtv.value());

        const auto [face_a, face_a_normal] =
                most_perpendicular_cube_face(manifold.normal, a.oriented_vertices);
        const auto [face_b, face_b_normal] =
                most_perpendicular_cube_face(-manifold.normal, b.oriented_vertices);

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
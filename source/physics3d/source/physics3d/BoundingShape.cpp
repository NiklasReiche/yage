#include "BoundingShape.h"
#include "Collision.h"

namespace physics3d
{
    double penetration_distance(const gml::Vec3d& p_a, const gml::Vec3d& p_b, gml::Vec3d n)
    {
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
        auto dist = gml::dot(a.center - b.support, b.normal); // projection does not need denominator, since normal is unit length
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
        auto dist = gml::dot(b.center - a.support, a.normal); // projection does not need denominator, since normal is unit length
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
}
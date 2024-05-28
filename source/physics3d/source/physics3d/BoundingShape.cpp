#include "BoundingShape.h"

namespace physics3d
{
    std::optional<CollisionContactManifold> CollisionVisitor::operator()(const BSphere& a, const BSphere& b)
    {
        auto ab = b.center - a.center;
        if (gml::sqrLength(ab) > (a.radius + b.radius) * (a.radius + b.radius)) {
            return {};
        }

        CollisionContact contact;
        contact.n = gml::normalize(b.center - a.center);
        contact.r_a = contact.n * a.radius;
        contact.p_a = a.center + contact.r_a;
        contact.r_b = -contact.n * a.radius;
        contact.p_b = b.center + contact.r_b;

        CollisionContactManifold result;
        result.contact = contact;
        return {result};
    }

    std::optional<CollisionContactManifold> CollisionVisitor::operator()(const BSphere& a, const BPlane& b)
    {
        // dist is positive if the circle collides on the outer side and negative otherwise (b.normal points outward)
        auto dist = gml::dot(a.center - b.support, b.normal); // projection does not need denominator, since normal is unit length
        if (std::abs(dist) > a.radius) {
            return {};
        }

        CollisionContact contact;
        contact.n = dist > 0 ? -b.normal : b.normal;
        contact.r_a = contact.n * a.radius;
        contact.p_a = a.center + contact.r_a;
        contact.p_b = a.center + contact.n * dist;
        contact.r_b = contact.p_a - b.support;

        CollisionContactManifold result;
        result.contact = contact;
        return {result};
    }

    std::optional<CollisionContactManifold> CollisionVisitor::operator()(const BPlane& a, const BSphere& b)
    {
        return operator()(b, a);
    }

    std::optional<CollisionContactManifold> CollisionVisitor::operator()(const BPlane& a, const BPlane& b)
    {
        return {}; // TODO
    }
}
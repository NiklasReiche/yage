#pragma once

#include <vector>
#include <optional>

#include <math/vector.h>
#include <math/matrix.h>

namespace yage::physics3d
{
    class RigidBody;

    /**
     * Represents a point of contact between two colliding bodies.
     */
    struct ContactPoint
    {
        /**
         * Relative offset of the contact point on object A from the center of object A.
         */
        math::Vec3d r_a;

        /**
         * World-space position of the contact point on object A.
         */
        math::Vec3d p_a;

        /**
         * Relative offset of the contact point on object B from the center of object B.
         */
        math::Vec3d r_b;

        /**
         * World-space position of the contact point on object B.
         */
        math::Vec3d p_b;

        /**
         * Penetration depth of the contact point along the contact normal.
         */
        double depth{};

        /**
         * Relative velocity from object A to B at the contact point.
         */
        math::Vec3d rel_v{};

        /**
         * Relative velocity from object A to B at the contact point along the contact normal.
         */
        double rel_v_n{};
    };

    /**
     * Represents a collision containing one or more contact points between two colliding bodies.
     */
    struct ContactManifold
    {
        /**
         * Points of contact for this collision. Must contain at east one contact point.
         */
        std::vector<ContactPoint> contacts;

        /**
         * Normalized contact normal vector from object A to B.
         */
        math::Vec3d normal;

        /**
         * Normalized contact tangent vector spanning the contact plane.
         */
        math::Vec3d tangent_1;

        /**
         * Normalized contact tangent vector spanning the contact plane.
         */
        math::Vec3d tangent_2;
    };

    /**
     * Represent a singular constraint between two bodies to be solved by the constraint solver.
     */
    struct Constraint
    {
        /**
         * The inverted mass matrix.
         */
        const math::Matd<12, 12> m_inv;

        /**
         * The Jacobian.
         */
        const math::Matd<1, 12> j;

        /**
         * The transpose of the Jacobian.
         */
        const math::Matd<12, 1> j_t;

        const double bias{};

        RigidBody& rb_a;
        RigidBody& rb_b;

        double accumulated_lambda{};

        /**
         * An optional constraint that this constraint depends on.
         */
        std::optional<const Constraint *> dependent_constraint{};
    };
}

#pragma once

#include <vector>
#include <functional>

#include "RigidBody.h"
#include "Collision.h"

namespace physics3d
{
    class Simulation
    {
    public:
        void integrate(double dt);

        template<typename... Args>
        std::shared_ptr<RigidBody> create_rigid_body(Args&& ... args)
        {
            bodies.push_back(std::make_shared<RigidBody>(args...));
            return bodies.back();
        }

        void enable_gravity();

	private:
        const double m_baumgarte_factor = 0.2;
        const double m_penetration_slop = 0.0001; // allowed penetration distance in meters
        const double m_restitution_slop = 0.2; // allowed relative velocity in the normal direction
        const int m_solver_iterations = 10;

        std::vector<std::shared_ptr<RigidBody>> bodies;
        CollisionVisitor m_collision_visitor{};
        gml::Vec3d m_external_acceleration{};

        double solve_constraint(Constraint& constraint);

        void apply_impulse(const Constraint& constraint, gml::Matd<12, 1> impulse);

        Constraint prepare_penetration_constraint(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold,
                                                  const ContactPoint& contact, double dt);

        std::tuple<Constraint, Constraint>
        prepare_friction_constraints(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold,
                                     const ContactPoint& contact);

        Constraint
        prepare_spinning_friction_constraint(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold);

        std::tuple<Constraint, Constraint>
        prepare_rolling_friction_constraints(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold);

        static double
        solve(gml::Matd<12, 12> m_inv, gml::Matd<1, 12> j, gml::Matd<12, 1> j_t, gml::Matd<12, 1> q_pre, double b);

        static gml::Matd<12, 12> inverse_mass_matrix(RigidBody& a, RigidBody& b);

        static std::tuple<gml::Vec3d, gml::Vec3d> tangent_plane(const gml::Vec3d& n);
    };
}

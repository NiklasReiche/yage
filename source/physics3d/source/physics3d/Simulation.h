#pragma once

#include <vector>
#include <functional>

#include "Particle.h"
#include "RigidBody.h"

namespace physics3d
{
    struct Collision
    {
        CollisionContactManifold contact_manifold;
        RigidBody& rb_a;
        RigidBody& rb_b;
        double depth{};
        gml::Vec3d rel_v{};
    };

	class Simulation
	{
	public:
		void integrate(double dt);

		Particle& addParticle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass);

        // TODO: make this a factory method
		void addRigidBody(const std::shared_ptr<RigidBody>& rigidBody);

        void enable_gravity();

	private:
        const double m_baumgarte_factor = 0.2;
        const double m_penetration_slop = 0.0005; // allowed penetration distance in meters
        const double m_restitution_slop = 0.0; // allowed relative velocity in the normal direction
        const int m_solver_iterations = 10;

		std::vector<Particle> particles;
		std::vector<std::shared_ptr<RigidBody>> bodies;
        CollisionVisitor m_collision_visitor{};
        gml::Vec3d m_external_acceleration{};

        /**
         * @param baumgarte_factor Bias factor for Baumgarte stabilisation. Values between 0.1 and 0.3 net good results.
         * @param dt Delta time of the integration step.
         */
        void resolve_collision(Collision& collision, double dt);

        static double solve(gml::Matd<12, 12> m_inv, gml::Matd<1, 12> j, gml::Matd<12, 1> j_t, gml::Matd<12, 1> q_pre, double b);

        static gml::Matd<12, 12> inverse_mass_matrix(RigidBody& a, RigidBody& b);

        static std::tuple<gml::Vec3d, gml::Vec3d> tangent_plane(const gml::Vec3d& n);
    };
}

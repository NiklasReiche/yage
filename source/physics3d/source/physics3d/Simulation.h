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
		std::vector<Particle> particles;
		std::vector<std::shared_ptr<RigidBody>> bodies;
        CollisionVisitor m_collision_visitor{};
        gml::Vec3d m_external_acceleration{};

        void resolve_collision(const Collision& collision, double bias, double dt);

        static double solve(gml::Matd<12, 12> m_inv, gml::Matd<1, 12> j, gml::Matd<12, 1> j_t, gml::Matd<12, 1> q_pre, double b);

        static gml::Matd<12, 12> inverse_mass_matrix(RigidBody& a, RigidBody& b);

        static std::tuple<gml::Vec3d, gml::Vec3d> tangent_plane(const gml::Vec3d& n);
    };
}

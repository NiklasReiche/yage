#pragma once

#include <vector>
#include <functional>

#include "Particle.h"
#include "RigidBody.h"

namespace physics3d
{
	class Simulation
	{
	public:
		void integrate(double dt);

		Particle& addParticle(const gml::Vec3d& position, const gml::Vec3d& velocity, double mass);

		void addRigidBody(const std::shared_ptr<RigidBody>& rigidBody);

	private:
		std::vector<Particle> particles;
		std::vector<std::shared_ptr<RigidBody>> bodies;

        void resolve(RigidBody& a, RigidBody& b);

        void resolve_constraint(RigidBody& a, RigidBody& b);

        void solve(gml::Matd<12, 12> m_inv, gml::Matd<1, 12> j, gml::Matd<12, 1> q_pre,
                   gml::Matd<12, 1>& delta_q, double& lambda,
                   const std::function<double(double)>& lambda_clamp = [](double l){return l;});

        gml::Matd<12, 12> inverse_mass_matrix(RigidBody& a, RigidBody& b);
    };
}

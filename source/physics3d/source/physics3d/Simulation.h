#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "RigidBody.h"
#include "Collision.h"
#include "Visualizer.h"

namespace yage::physics3d
{
    /**
     * Performs a discrete physics simulation on a set of rigid bodies. Implements a Sequential Impulses approach with
     * support for penetration, friction, rolling friction, and restitution.
     */
    class Simulation
    {
    public:
        Simulation() = default;

        explicit Simulation(Visualizer visualizer);

        /**
         * Performs one full simulation step. The order of operation is:
         *  1. Integrations of forces
         *  2. Collision detection
         *  3. Collision resolution
         *  4. Integration of positions
         * @param dt Simulation delta time for this step in seconds.
         */
        void update(double dt);

        /**
         * Performs one full simulation step. The order of operation is:
         *  1. Collision resolution (of collisions detected in the previous frame)
         *  2. Integration of positions
         *  3. Integrations of forces
         *  4. Collision detection (stored for next frame)
         * @param dt Simulation delta time for this step in seconds.
         */
        void update_staggered(double dt);

        /**
         * Creates a rigid body and adds it to the simulation. To remove the body from the simulation, use its destroy
         * method.
         * @param args Constructor arguments for the rigid body.
         * @return A pointer to the constructed rigid body.
         */
        template<typename... Args>
        std::shared_ptr<RigidBody> create_rigid_body(Args&& ... args)
        {
            bodies.push_back(std::make_shared<RigidBody>(args...));
            return bodies.back();
        }

        void enable_gravity();

        void disable_gravity();

        void visualize_collisions(const math::Mat4d& projection, const math::Mat4d& view) const;

    private:
        /**
         * Baumgarte stabilisation factor. Should be within [0.1, 0.3].
         */
        double m_baumgarte_factor = 0.2;
        /**
         * Ignored penetration distance when constraint solving in meters.
         */
        double m_penetration_slop = 0.0001;
        /**
         * Ignored relative velocity in the normal direction when constraint solving in meters/second.
         */
        double m_restitution_slop = 0.2;
        /**
         * Maximum number of iterations per frame for the Sequential Impulses solver.
         */
        int m_solver_iterations = 10;

        std::vector<std::shared_ptr<RigidBody>> bodies;
        CollisionVisitor m_collision_visitor{};
        math::Vec3d m_external_acceleration{};

        std::vector<Constraint> m_penetration_constraints;
        std::vector<Constraint> m_friction_constraints;
        std::vector<Constraint> m_rolling_friction_constraints;

        std::unique_ptr<Visualizer> m_visualizer;

        void integrate_forces(double dt);

        void integrate_positions(double dt);

        void detect_collisions(double dt);

        void resolve_collisions(double dt);

        void remove_destroyed_bodies();

        void clear_forces();

        static double solve_constraint(const Constraint& constraint);

        static void apply_impulse(const Constraint& constraint, const math::Matd<12, 1>& impulse);

        Constraint
        prepare_penetration_constraint(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold,
                                       const ContactPoint& contact, double dt) const;

        static std::tuple<Constraint, Constraint>
        prepare_friction_constraints(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold,
                                     const ContactPoint& contact);

        static std::tuple<Constraint, Constraint, Constraint>
        prepare_rolling_friction_constraints(RigidBody& rb_a, RigidBody& rb_b, const ContactManifold& manifold);

        static void resolve_penetration_constraint(Constraint& constraint);

        static void resolve_friction_constraint(Constraint& constraint);

        static void resolve_rolling_friction_constraint(Constraint& constraint);

        static math::Matd<12, 12> inverse_mass_matrix(const RigidBody& a, const RigidBody& b);

        static std::tuple<math::Vec3d, math::Vec3d> tangent_plane(const math::Vec3d& n);
    };
}

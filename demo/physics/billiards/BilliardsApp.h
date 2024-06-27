#pragma once

#include <runtime/Application.h>
#include <runtime/Engine.h>

#include "AppListener.h"

class BilliardsApp final : public yage::Application
{
public:
    BilliardsApp() = default;

    void initialize() override;

    void pre_render_update() override;

    void hit_ball();

    void reset();

    void step_simulation() const;

    void toggle_simulation() const;

    void toggle_visualization() const;

    void toggle_mouse_capture();

private:
    const double billiard_ball_radius = 0.03;
    const double billiard_ball_mass = 0.17;
    int n_barriers = 0;
    int n_balls = 0;
    yage::math::Vec3d table_position{};

    yage::physics3d::Material billiard_ball_material{
            .restitution = 0.99,
            .kinetic_friction = 0.2,
            .rolling_friction = 0.001,
    };

    yage::physics3d::Material billiard_table_material{
            .restitution = 0.9,
            .kinetic_friction = 1.0,
            .rolling_friction = 1.0,
    };

    std::optional<yage::gl3d::MeshResource> ball_mesh;
    std::optional<yage::gl3d::MeshResource> barrier_mesh;
    std::optional<yage::gl3d::MeshResource> ground_mesh;

    std::optional<std::reference_wrapper<yage::GameObject>> player_ball;

    std::vector<yage::GameObject> objects;
    MovementListener app_listener;

    yage::GameObject&
    load_ball(yage::math::Vec3d position);

    void setup_balls(const yage::math::Vec3d& offset);

    void load_barrier(const yage::math::Vec3d& position, const yage::math::Quatd& orientation, const yage::math::Vec3d& scale = yage::math::Vec3d(1));

    void load_ground(const yage::math::Vec3d& position);

    void setup_lights() const;

    void load_gui();
};

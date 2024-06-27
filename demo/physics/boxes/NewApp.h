#pragma once

#include <math/math.h>
#include <runtime/Application.h>
#include <runtime/Engine.h>

#include "AppListener.h"

class NewApp final : public yage::Application
{
public:
    NewApp() = default;

    void initialize() override;

    void pre_render_update() override;

    void add_box();

    void throw_box();

    void add_box_rotated();

    void reset();

    void step_simulation() const;

    void toggle_simulation() const;

    void toggle_visualization() const;

    void toggle_mouse_capture();

private:
    int box_stack = 0;
    int box_mass = 1;
    const yage::math::Vec3d box_offset = yage::math::Vec3d(0, 1.2, 0);
    const int box_length = 2;
    int n_cubes = 0;
    yage::physics3d::Material ground_material{
        .restitution = 0.0,
        .kinetic_friction = 1.0,
    };
    yage::physics3d::Material cube_material{
        .restitution = 0.0,
        .kinetic_friction = 0.5,
    };

    std::optional<yage::res::Resource<std::unique_ptr<yage::gl3d::Mesh>>> cube_mesh;
    std::vector<yage::physics3d::RigidBodyHandle> rigid_bodies;
    AppListener app_listener;

    yage::GameObject
    load_cube(const std::string& filename, yage::math::Vec3d position, yage::math::Quatd orientation = yage::math::Quatd());

    void load_ground();

    void setup_lights() const;
};

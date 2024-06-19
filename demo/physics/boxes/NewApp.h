#pragma once
#include "runtime/Application.h"
#include <math/math.h>

#include "core/platform/desktop/FileReader.h"
#include "gl3d/resources/gltf.h"
#include "runtime/Engine.h"

using namespace yage;

class NewApp final : public yage::Application
{
public:
    NewApp()
    {
    }

    void add_box()
    {
        const double height = box_stack * (box_length + 0.5);
        load_cube("models/box.glb", box_offset + yage::math::Vec3d(0, height, 0));
        box_stack++;
    }

    void throw_box()
    {
        const auto go = load_cube("models/box.glb", box_offset + yage::math::Vec3d(-20, 10, 0));
        go.rigid_body->apply_force(yage::math::Vec3d(800, 1000, 0), go.rigid_body->position() + yage::math::Vec3d(0.2, 0, 0.1));
    }

    void add_box_rotated()
    {
        const double height = box_stack * (box_length + 0.5);
        load_cube("models/box.glb", box_offset + yage::math::Vec3d(0, height, 0),
                  yage::math::quaternion::euler_angle<double>(0.0, yage::math::to_rad(10.0), yage::math::to_rad(30.0)));
        box_stack++;
    }

private:
    int box_stack = 0;
    int box_mass = 1;
    const yage::math::Vec3d box_offset = yage::math::Vec3d(0, 1.2, 0);
    const int box_length = 2;
    int n_cubes = 0;
    physics3d::Material ground_material{
        .restitution = 0.0,
        .kinetic_friction = 1.0,
    };
    physics3d::Material cube_material{
        .restitution = 0.0,
        .kinetic_friction = 0.5,
    };

    std::shared_ptr<gl3d::Mesh> cube_mesh;

    GameObject
    load_cube(const std::string& filename, math::Vec3d position, math::Quatd orientation = math::Quatd())
    {
        if (!cube_mesh) {
            cube_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                          filename, *m_engine->gl_context().getDrawableCreator(),
                                                          *m_engine->gl_context().getTextureCreator(),
                                                          pbrShader,
                                                          pbrShaderNormalMapping).at(0);
        }

        auto game_object = m_engine->register_game_object("cube" + n_cubes++);

        game_object.scene_node = m_engine->scene->create_object("cube" + n_cubes++);
        game_object.scene_node.value().get().mesh = cube_mesh;

        game_object.rigid_body = m_engine->physics.create_rigid_body(
            physics3d::InertiaShape::cube(2, box_mass),
            physics3d::colliders::OrientedBox{
                .half_size = math::Vec3d(1, 1, 1),
            },
            cube_material,
            position,
            orientation);

        return game_object;
    }
};

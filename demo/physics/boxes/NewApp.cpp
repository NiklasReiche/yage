#include "NewApp.h"

using namespace yage;

void NewApp::initialize()
{
    load_ground();
    setup_lights();
    add_box();
    add_box();
    add_box();
    add_box();

    m_engine->render_camera.move_to(math::Vec3d(25.0, 25.0, 25.0));
    m_engine->render_camera.rotate_to(
        math::quaternion::euler_angle<double>(math::to_rad(225.), 0, 0) *
        math::quaternion::euler_angle<double>(0, 0, math::to_rad(30.)));

    m_engine->physics.enable_gravity();
}

void NewApp::add_box()
{
    const double height = box_stack * (box_length + 0.5);
    load_cube("models/box.glb", box_offset + yage::math::Vec3d(0, height, 0));
    box_stack++;
}

void NewApp::throw_box()
{
    const auto& [_, rigid_body] =
        load_cube("models/box.glb", box_offset + yage::math::Vec3d(-20, 10, 0));
    rigid_body->apply_force(yage::math::Vec3d(800, 1000, 0),
                               rigid_body->position() + yage::math::Vec3d(0.2, 0, 0.1));
}

void NewApp::add_box_rotated()
{
    const double height = box_stack * (box_length + 0.5);
    load_cube("models/box.glb", box_offset + yage::math::Vec3d(0, height, 0),
              yage::math::quaternion::euler_angle<double>(0.0, yage::math::to_rad(10.0), yage::math::to_rad(30.0)));
    box_stack++;
}

GameObject NewApp::load_cube(const std::string& filename, math::Vec3d position, math::Quatd orientation)
{
    if (!cube_mesh) {
        cube_mesh = m_engine->mesh_store.loadResource(*m_engine->mesh_loader, filename);
    }

    auto& game_object = m_engine->register_game_object("cube" + std::to_string(n_cubes));

    game_object.scene_node = m_engine->scene->create_object("cube" + n_cubes);
    game_object.scene_node.value().get().mesh = cube_mesh;

    game_object.rigid_body = m_engine->physics.create_rigid_body(
        physics3d::InertiaShape::cube(2, box_mass),
        physics3d::colliders::OrientedBox{
            .half_size = math::Vec3d(1, 1, 1),
        },
        cube_material,
        position,
        orientation);

    n_cubes++;
    return game_object;
}

void NewApp::load_ground()
{
    const auto mesh = m_engine->mesh_store.loadResource(*m_engine->mesh_loader, std::string{"models/ground.glb"});

    auto& [scene_node, rigid_body] = m_engine->register_game_object("ground");

    scene_node = m_engine->scene->create_object("ground");
    scene_node.value().get().mesh = mesh;

    rigid_body = m_engine->physics.create_rigid_body(
        physics3d::InertiaShape::static_shape(),
        physics3d::colliders::OrientedPlane{
            .original_normal = {0, -1, 0},
        },
        ground_material,
        math::Vec3d(0),
        math::Quatd());
}

void NewApp::setup_lights() const
{
    const auto lightRes = std::make_shared<gl3d::PointLight>();
    lightRes->color = math::Vec3f(100);

    auto& light = m_engine->scene->create_object("light1");
    light.light = lightRes;
    light.local_transform =
            math::matrix::translate<double>(-10, 10, -10);

    const auto lightRes2 = std::make_shared<gl3d::DirectionalLight>();
    lightRes2->color = math::Vec3f(3);

    auto& light2 = m_engine->scene->create_object("light2");
    light2.light = lightRes2;
    light2.local_transform =
            math::matrix::from_quaternion<double>(
                math::quaternion::euler_angle<double>(math::to_rad(200.), 0, 0) *
                math::quaternion::euler_angle<double>(0, 0, math::to_rad(60.))
            );
}

#include "NewApp.h"
#include <gui/gui.h>

using namespace yage;

void NewApp::initialize()
{
    load_gui();

    load_ground();
    setup_lights();
    add_box();
    add_box();
    add_box();
    add_box();

    const std::shared_ptr<gl3d::Camera> camera = std::make_shared<gl3d::Camera>();
    camera->move_to(math::Vec3d(25.0, 25.0, 25.0));
    camera->rotate_to(math::quaternion::euler_angle<double>(math::to_rad(225.), 0, 0) *
                      math::quaternion::euler_angle<double>(0, 0, math::to_rad(30.)));
    m_engine->scene_renderer.active_camera = camera;

    m_engine->physics.enable_gravity();

    app_listener = AppListener(camera, this);
    m_engine->register_input_listener(app_listener);
}

void NewApp::pre_render_update()
{
    app_listener.update();
}

void NewApp::add_box()
{
    const double height = box_stack * (box_length + 0.5);
    load_cube("models/box.glb", box_offset + math::Vec3d(0, height, 0));
    box_stack++;
}

void NewApp::throw_box()
{
    const auto& [_, rigid_body_handle] = load_cube("models/box.glb", box_offset + math::Vec3d(-20, 10, 0));
    physics3d::RigidBody& rigid_body = m_engine->physics.lookup(rigid_body_handle.value());

    rigid_body.apply_force(math::Vec3d(800, 1000, 0), rigid_body.position() + math::Vec3d(0.2, 0, 0.1));
}

void NewApp::add_box_rotated()
{
    const double height = box_stack * (box_length + 0.5);
    load_cube("models/box.glb", box_offset + math::Vec3d(0, height, 0),
              math::quaternion::euler_angle<double>(0.0, math::to_rad(10.0), math::to_rad(30.0)));
    box_stack++;
}

void NewApp::reset()
{
    for (const physics3d::RigidBodyHandle& rb: rigid_bodies) {
        m_engine->physics.lookup(rb).destroy();
    }
    rigid_bodies.clear();

    m_engine->scene_renderer.active_scene = std::make_shared<gl3d::SceneGroup>("root");

    box_stack = 0;

    load_ground();
    setup_lights();
}

void NewApp::step_simulation() const
{
    if (m_engine->enable_physics_simulation)
        return;

    if (m_engine->enable_physics_visualization) {
        m_engine->physics.update_staggered(1. / 60);
    } else {
        m_engine->physics.update(1. / 60.);
    }
}

void NewApp::toggle_simulation() const
{
    m_engine->enable_physics_simulation = !m_engine->enable_physics_simulation;
}

void NewApp::toggle_visualization() const
{
    m_engine->enable_physics_visualization = !m_engine->enable_physics_visualization;
}

void NewApp::toggle_mouse_capture()
{
    m_engine->toggle_cursor_visibility();
}

GameObject NewApp::load_cube(const std::string& filename, math::Vec3d position, math::Quatd orientation)
{
    if (!cube_mesh) {
        cube_mesh = m_engine->mesh_store.loadResource(*m_engine->mesh_loader, filename);
    }

    auto& game_object = m_engine->register_game_object("cube" + std::to_string(n_cubes));

    game_object.scene_node = m_engine->scene_renderer.active_scene->create_object("cube" + n_cubes);
    game_object.scene_node.value().get().mesh = cube_mesh;

    game_object.rigid_body = m_engine->physics.create_rigid_body(physics3d::InertiaShape::cube(2, box_mass),
                                                                 physics3d::colliders::OrientedBox{
                                                                         .half_size = math::Vec3d(1, 1, 1),
                                                                 },
                                                                 cube_material, position, orientation);

    n_cubes++;
    return game_object;
}

void NewApp::load_ground()
{
    const auto mesh = m_engine->mesh_store.loadResource(*m_engine->mesh_loader, std::string{"models/ground.glb"});

    auto& [scene_node, rigid_body] = m_engine->register_game_object("ground");

    scene_node = m_engine->scene_renderer.active_scene->create_object("ground");
    scene_node.value().get().mesh = mesh;

    rigid_body = m_engine->physics.create_rigid_body(physics3d::InertiaShape::static_shape(),
                                                     physics3d::colliders::OrientedPlane{
                                                             .original_normal = {0, -1, 0},
                                                     },
                                                     ground_material, math::Vec3d(0), math::Quatd());
}

void NewApp::setup_lights() const
{
    const auto lightRes = std::make_shared<gl3d::PointLight>();
    lightRes->color = math::Vec3f(100);

    auto& light = m_engine->scene_renderer.active_scene->create_object("light1");
    light.light = lightRes;
    light.local_transform = math::matrix::translate<double>(-10, 10, -10);

    const auto lightRes2 = std::make_shared<gl3d::DirectionalLight>();
    lightRes2->color = math::Vec3f(3);

    auto& light2 = m_engine->scene_renderer.active_scene->create_object("light2");
    light2.light = lightRes2;
    light2.local_transform =
            math::matrix::from_quaternion<double>(math::quaternion::euler_angle<double>(math::to_rad(200.), 0, 0) *
                                                  math::quaternion::euler_angle<double>(0, 0, math::to_rad(60.)));
}

void NewApp::load_gui()
{
    const auto font =
            m_engine->font_store.loadResource(*m_engine->font_loader, std::string{"fonts/OpenSans-Regular.font"});

    const auto frame = m_engine->gui
                               .create_widget<gui::ListBox>(
                                       gui::ListBoxTemplate{
                                               .base =
                                                       {
                                                               .geometry =
                                                                       {
                                                                               .size_hint = math::Vec2(
                                                                                       gui::SizeHint::FIT_CHILDREN),
                                                                       },
                                                               .border = {.thickness = 1},
                                                               .color = gl::Color::WHITE,
                                                               .padding = {2, 2, 2, 2},
                                                       },
                                               .orientation = gui::ListBoxTemplate::VERTICAL,
                                       });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"Controls:",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"x: toggle mouse capture for looking around",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"mouse: look around",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"w,a,s,d: move around",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"space: toggle simulation",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"b: spawn box",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"n: spawn rotated box",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"enter: spawn moving box",
                            .font = font,
                    },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text =
                    {
                            .text = U"v: toggle contact point visualizer",
                            .font = font,
                    },
    });
}

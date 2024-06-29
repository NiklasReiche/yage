#include "BilliardsApp.h"
#include <gui/gui.h>

using namespace yage;

void BilliardsApp::initialize()
{
    ground_mesh = m_engine->mesh_store.load_resource(std::string{"models/ground.glb:0"});
    barrier_mesh = m_engine->mesh_store.load_resource(std::string{"models/barrier.glb:0"});
    ball_mesh = m_engine->mesh_store.load_resource(std::string{"models/old_billiard_ball.glb:0"});

    load_gui();

    setup_scene();

    const std::shared_ptr<gl3d::Camera> camera = std::make_shared<gl3d::Camera>();
    camera->look_at(math::Vec3d(0.0, 2.0, 2.0), math::Vec3d(0.0));
    m_engine->scene_renderer.active_camera = camera;

    m_engine->physics.enable_gravity();

    app_listener = AppListener(camera, this);
    m_engine->register_input_listener(app_listener);
}

void BilliardsApp::pre_render_update()
{
    app_listener.update();
}

void BilliardsApp::hit_ball()
{
    physics3d::RigidBody& rigid_body = m_engine->physics.lookup(player_ball.value().get().rigid_body.value());
    rigid_body.apply_force(math::Vec3d(35, 0, 0), rigid_body.position());
}

void BilliardsApp::reset()
{
    for (const GameObject& game_object: objects) {
        m_engine->destroy_game_object(game_object);
    }
    objects.clear();

    n_barriers = 0;
    n_balls = 0;

    setup_scene();
}

void BilliardsApp::step_simulation() const
{
    if (m_engine->enable_physics_simulation)
        return;

    if (m_engine->enable_physics_visualization) {
        m_engine->physics.update_staggered(1. / 60);
    } else {
        m_engine->physics.update(1. / 60.);
    }
}

void BilliardsApp::toggle_simulation() const
{
    m_engine->enable_physics_simulation = !m_engine->enable_physics_simulation;
}

void BilliardsApp::toggle_visualization() const
{
    m_engine->enable_physics_visualization = !m_engine->enable_physics_visualization;
}

void BilliardsApp::toggle_mouse_capture()
{
    m_engine->toggle_cursor_visibility();
}

void BilliardsApp::load_barrier(const math::Vec3d& position, const math::Quatd& orientation, const math::Vec3d& scale)
{
    GameObject& game_object = m_engine->register_game_object("barrier" + std::to_string(n_barriers));

    game_object.scene_node =
            m_engine->scene_renderer.active_scene.value().get().create_object("barrier" + std::to_string(n_barriers));
    game_object.scene_node.value().get().mesh = barrier_mesh;
    game_object.scene_node.value().get().local_transform = math::matrix::scale(scale);

    game_object.rigid_body = m_engine->physics.create_rigid_body(physics3d::InertiaShape::static_shape(),
                                                                 physics3d::colliders::OrientedPlane{
                                                                         .original_normal = {0, 0, -1},
                                                                 },
                                                                 billiard_table_material, position, orientation);

    objects.push_back(game_object);
    n_barriers++;
}

void BilliardsApp::load_ground(const math::Vec3d& position)
{
    GameObject& game_object = m_engine->register_game_object("ground");

    game_object.scene_node = m_engine->scene_renderer.active_scene.value().get().create_object("ground");
    game_object.scene_node.value().get().mesh = ground_mesh;
    game_object.scene_node.value().get().local_transform = math::matrix::scale(1.0, 1.0, 0.5);

    game_object.rigid_body = m_engine->physics.create_rigid_body(physics3d::InertiaShape::static_shape(),
                                                                 physics3d::colliders::OrientedPlane{
                                                                         .original_normal = {0, -1, 0},
                                                                 },
                                                                 billiard_table_material, position, math::Quatd());

    objects.push_back(game_object);
}

GameObject& BilliardsApp::load_ball(math::Vec3d position)
{
    GameObject& game_object = m_engine->register_game_object("ball" + std::to_string(n_balls));

    game_object.scene_node = m_engine->scene_renderer.active_scene.value().get().create_object("ball" + std::to_string(n_balls));
    game_object.scene_node.value().get().mesh = ball_mesh;
    game_object.scene_node.value().get().local_transform =
            math::matrix::scale<double>(math::Vec3d(billiard_ball_radius));

    game_object.rigid_body = m_engine->physics.create_rigid_body(
    physics3d::InertiaShape::sphere(billiard_ball_radius, billiard_ball_mass),
            physics3d::colliders::Sphere{
                    .radius = billiard_ball_radius,
            },
            billiard_ball_material, position,
            math::quaternion::from_matrix(
                    math::matrix::axisAngle(math::Vec3d(0, 0, 1), std::numbers::pi_v<double> / 2).rotation()));

    objects.push_back(game_object);
    n_balls++;
    return game_object;
}

void BilliardsApp::setup_balls(const math::Vec3d& offset)
{
    const double height = std::sqrt(-5 * 0.5 * 2 * billiard_ball_radius * 5 * 0.5 * 2 * billiard_ball_radius +
                                    5 * 2 * billiard_ball_radius * 5 * 2 * billiard_ball_radius) /
                          5.;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            constexpr double epsilon = 0.00000001;
            load_ball(offset +
                      math::Vec3d(i * height + epsilon * i, 0,
                                  -(i * billiard_ball_radius * 2) / 2.0 + j * billiard_ball_radius * 2 + j * epsilon));
        }
    }
}

void BilliardsApp::setup_lights() const
{
    const auto lightRes = std::make_shared<gl3d::PointLight>();
    lightRes->light_model = gl3d::PbrLightModel{
        .color = math::Vec3f(30)
    };

    auto& light = m_engine->scene_renderer.active_scene.value().get().create_object("light1");
    light.light = lightRes;
    light.local_transform = math::matrix::translate<double>(0, 3, 0);
}

void BilliardsApp::load_gui()
{
    const auto font =
            m_engine->font_store.load_resource(std::string{"fonts/OpenSans-Regular.font"});

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
                            .text = U"enter: shoot ball",
                            .font = font,
                    },
    });
}

void BilliardsApp::setup_scene()
{
    m_engine->scene_renderer.active_scene = m_engine->scene_store.load_resource("");

    setup_lights();
    load_ground(math::Vec3d(0));
    load_barrier(math::Vec3d(1, 0, 0), math::quaternion::euler_angle<double>(-std::numbers::pi_v<double> / 2, 0, 0),
                 math::Vec3d(0.5, 1.0, 1.0));
    load_barrier(math::Vec3d(0, 0, 0.5), math::quaternion::euler_angle<double>(std::numbers::pi_v<double>, 0, 0));
    load_barrier(math::Vec3d(0, 0, -0.5), math::Quatd());
    load_barrier(math::Vec3d(-1, 0, 0), math::quaternion::euler_angle<double>(std::numbers::pi_v<double> / 2, 0, 0),
                 math::Vec3d(0.5, 1.0, 1.0));
    setup_balls(math::Vec3d(0.35, table_position.y() + billiard_ball_radius + 0.00001, 0));
    player_ball = load_ball(math::Vec3d(-0.5, table_position.y() + billiard_ball_radius + 0.00001, 0));
}

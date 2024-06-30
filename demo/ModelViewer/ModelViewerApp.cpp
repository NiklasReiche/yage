#include "ModelViewerApp.h"

#include <gl3d/light.h>
#include <runtime/Engine.h>
#include <gui/gui.h>

using namespace yage;

void ModelViewerApp::initialize()
{
    m_engine->scene_renderer.active_camera = std::make_shared<gl3d::Camera>();
    m_engine->scene_renderer.active_camera->look_at(math::Vec3d(400.0), math::Vec3d(0));

    inputListener = MovementListener(this, m_engine->scene_renderer.active_camera);
    m_engine->register_input_listener(inputListener);

    m_engine->scene_renderer.active_scene = m_engine->scene_store.load_resource("");
    setup_gui();
    setup_light();
}

void ModelViewerApp::scale_scene(const double factor)
{
    m_engine->scene_renderer.active_scene.value().get().local_transform *=
        math::matrix::scale<double>(factor, factor, factor);
}


void ModelViewerApp::on_button_click()
{
    const std::string path = m_engine->select_file();
    if (!path.empty()) {
        load_scene(path);
    }
}

void ModelViewerApp::load_scene(const std::string& filename)
{
    m_engine->scene_renderer.active_scene = m_engine->scene_store.load_resource(filename);
    setup_light();
}

void ModelViewerApp::setup_light()
{
    const auto light = std::make_shared<gl3d::DirectionalLight>();
    light->light_models.emplace_back(gl3d::PbrLightModel{
        .color = math::Vec3f(5, 5, 5)
    });
    light->light_models.emplace_back(gl3d::PhongLightModel{
        .ambient = math::Vec3f(0.2, 0.2, 0.2),
        .diffuse = math::Vec3f(0.5, 0.5, 0.5),
        .specular = math::Vec3f(1, 1, 1)
    });

    gl3d::SceneObject& light_node = m_engine->scene_renderer.active_scene.value().get().create_object("light");
    light_node.local_transform = math::matrix::from_quaternion<double>(
        math::quaternion::euler_angle<double>(math::to_rad(180.0), math::to_rad(0.0), math::to_rad(45.0)));
    light_node.light = light;
}

void ModelViewerApp::setup_gui()
{
    const auto font = m_engine->font_store.load_resource("assets/fonts/OpenSans-Regular.font");
    const auto frame = m_engine->gui.create_widget<gui::ListBox>(gui::ListBoxTemplate{
        .base = {
            .geometry = {
                .size_hint = math::Vec2(gui::SizeHint::FIT_CHILDREN),
            },
            .color = gl::Color::WHITE,
            .padding = {2, 2, 2, 2},
        },
        .orientation = gui::ListBoxTemplate::VERTICAL,
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
        .text = {
            .text = U"Controls:",
            .font = font,
        },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
        .text = {
            .text = U"Zoom in/out: scroll mouse",
            .font = font,
        },
    });
    frame->create_widget<gui::Label>(gui::LabelTemplate{
        .text = {
            .text = U"Pan: left-click + drag mouse",
            .font = font,
        },
    });
    gui::PushButton* button = frame->create_widget<gui::PushButton>(gui::PushButtonTemplate{
        .base = {
            .geometry = {
                .size_hint = math::Vec2(gui::SizeHint::FIT_CHILDREN),
            },
            .border = {.thickness = 1},
            .color = gl::Color::LIGHT_BLUE,
            .padding = {2, 2, 2, 2},
        },
        .command = [this](){ on_button_click(); }
    });
    button->create_widget<gui::Label>(gui::LabelTemplate{
        .text = {
            .text = U"select scene",
            .font = font,
        },
    });
}

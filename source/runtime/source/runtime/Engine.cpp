#include "Engine.h"

#include <ranges>

#include <core/platform/desktop/GlfwWindow.h>
#include <gl3d/MeshLoader.h>
#include <gl3d/SceneLoader.h>

namespace yage
{
    Engine::Engine(int width, int height, const std::string& title) :
        m_window(std::make_unique<platform::desktop::GlfwWindow>(width, height, title, platform::desktop::GlfwWindow::GlApi::API_OPENGL)),
        m_gl_context(gl::createOpenGlContext(m_window)),
        scene_renderer(*m_gl_context),
        physics(physics3d::Visualizer(*m_gl_context)),
        gui(m_window, m_gl_context),
        mesh_store(std::make_unique<gl3d::MeshFileLoader>(m_window->getFileReader(), m_gl_context->getTextureCreator(),
            m_gl_context->getDrawableCreator(), scene_renderer.shaders())),
        scene_store(std::make_unique<gl3d::SceneLoader>(m_window->getFileReader(), &mesh_store)),
        font_store(std::make_unique<font::FontFileLoader>(m_gl_context->getTextureCreator(), m_window->getFileReader()))
    {
        scene_renderer.base_renderer().setViewport(0, 0, width, height);
        scene_renderer.base_renderer().setClearColor(0x008080FFu);
        scene_renderer.projection() = math::matrix::perspective<float>(
                45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
    }

    void Engine::run()
    {
        m_application->initialize();

        m_window->show();
        m_window->getTimeStep();

        constexpr double physics_dt = 1. / 60.;
        double physics_dt_accumulator = 0.0;

        constexpr double gui_dt = 1. / 60.;
        double gui_dt_accumulator = 0.0;

        while (!m_window->shouldDestroy()) {
            const double frame_time = m_window->getTimeStep();

            scene_renderer.base_renderer().setClearColor(gl::Color::WHITE);
            scene_renderer.base_renderer().clear();

            // simulate physics
            if (enable_physics_simulation) {
                physics_dt_accumulator += frame_time;
                while (physics_dt_accumulator >= physics_dt) {
                    m_application->pre_physics_update();
                    if (enable_physics_visualization) {
                        physics.update_staggered(physics_dt);
                    } else {
                        physics.update(physics_dt);
                    }
                    physics_dt_accumulator -= physics_dt;
                }
            }

            // update scene graph from rigid bodies
            for (GameObject& game_object : m_game_objects | std::ranges::views::values) {
                if (!game_object.scene_node || !game_object.rigid_body) {
                    continue;
                }

                physics3d::RigidBody& rigid_body = physics.lookup(game_object.rigid_body.value());

                game_object.scene_node.value().get().local_transform =
                        math::matrix::translate(rigid_body.position()) *
                        math::matrix::from_quaternion(rigid_body.orientation()) *
                        math::matrix::scale(game_object.scene_node.value().get().local_transform.scale());
            }

            // render scene
            scene_renderer.base_renderer().clear();
            m_application->pre_render_update();
            if (enable_physics_visualization) {
                scene_renderer.base_renderer().enableWireframe();
                scene_renderer.render_active_scene();
                scene_renderer.base_renderer().disableWireframe();

                physics.visualize_collisions(static_cast<math::Mat4d>(scene_renderer.projection()),
                                             static_cast<math::Mat4d>(scene_renderer.view()));
            } else {
                scene_renderer.render_active_scene();
            }

            // update gui
            gui_dt_accumulator += frame_time;
            while (gui_dt_accumulator >= gui_dt) {
                gui.update(gui_dt);
                gui_dt_accumulator -= gui_dt;
            }
            gui.render();

            m_window->swapBuffers();
            m_window->pollEvents();
        }
    }

    void Engine::register_input_listener(input::InputListener& input_listener)
    {
        m_window->attach(input_listener);
    }

    GameObject& Engine::register_game_object(const std::string& id)
    {
        m_game_objects[id].id = id;
        return m_game_objects[id];
    }

    void Engine::destroy_game_object(const GameObject& game_object)
    {
        if (game_object.rigid_body) {
            physics.lookup(game_object.rigid_body.value()).destroy();
        }
        // TODO: allow destroying individual scene graph nodes
        m_game_objects.erase(game_object.id);
    }

    void Engine::toggle_cursor_visibility()
    {
        m_window->toggleCursor();
    }

    gl::IContext& Engine::gl_context()
    {
        return *m_gl_context;
    }

    std::string Engine::select_file()
    {
        return m_window->openFileDialog("");
    }
}

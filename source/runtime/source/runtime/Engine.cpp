#include "Engine.h"

#include <ranges>

#include "core/platform/desktop/GlfwWindow.h"

namespace yage
{
    Engine::Engine(int width, int height, const std::string& title)
        : physics(physics3d::Visualizer()),
          m_window(std::make_unique<platform::desktop::GlfwWindow>(width, height, title))
    {
        m_gl_context = gl::createContext(m_window);

        m_scene_renderer = std::make_unique<gl3d::SceneRenderer>(*m_gl_context);
        m_scene_renderer->base_renderer().setViewport(0, 0, width, height);
        m_scene_renderer->base_renderer().setClearColor(0x008080FFu);

        m_scene_renderer->projection() = math::matrix::perspective<float>(
            45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

        mesh_loader = std::make_unique<gl3d::MeshFileLoader>(
            m_window->getFileReader(),
            m_gl_context->getTextureCreator(),
            m_gl_context->getDrawableCreator(),
            m_scene_renderer->shaders());

        scene = std::make_unique<gl3d::SceneGroup>("root");
    }

    void Engine::run()
    {
        m_application->initialize();

        m_window->show();
        m_window->getTimeStep();

        constexpr double dt = 1. / 60.;
        double accumulator = 0.0;
        while (!m_window->shouldDestroy()) {
            const double frame_time = m_window->getTimeStep();
            m_scene_renderer->base_renderer().setClearColor(gl::Color::WHITE);
            m_scene_renderer->base_renderer().clear();

            if (enable_physics_simulation) {
                accumulator += frame_time;
                while (accumulator >= dt) {
                    m_application->pre_physics_update();
                    if (enable_physics_visualization) {
                        physics.update_staggered(dt);
                    } else {
                        physics.update(dt);
                    }
                    accumulator -= dt;
                }
            }

            // update scene graph from rigid bodies
            for (auto& [scene_node, rigid_body]: m_game_objects | std::ranges::views::values) {
                if (!scene_node || !rigid_body) {
                    continue;
                }
                scene_node.value().get().local_transform =
                        math::matrix::translate(rigid_body->position()) *
                        math::matrix::from_quaternion(rigid_body->orientation()) *
                        math::matrix::scale(scene_node.value().get().local_transform.scale());
            }

            m_scene_renderer->base_renderer().clear();
            m_application->pre_render_update();
            m_scene_renderer->render_graph(scene, render_camera);

            m_window->swapBuffers();
            m_window->pollEvents();
        }
    }

    GameObject& Engine::register_game_object(const std::string& id)
    {
        return m_game_objects[id];
    }

    gl::IContext& Engine::gl_context()
    {
        return *m_gl_context;
    }
}

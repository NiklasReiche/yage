#include "Engine.h"
#include "core/platform/desktop/GlfwWindow.h"

namespace yage
{
    Engine::Engine(int width, int height, const std::string& title)
        : physics(physics3d::Visualizer()),
          m_window(std::make_unique<platform::desktop::GlfwWindow>(width, height, title))
    {
        m_gl_context = gl::createContext(m_window);
    }

    void Engine::run()
    {
        m_window->show();
        m_window->getTimeStep();

        constexpr double dt = 1. / 60.;
        double accumulator = 0.0;
        while (!m_window->shouldDestroy()) {
            const double frame_time = m_window->getTimeStep();

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

            m_scene_renderer->base_renderer().clear();
            m_application->pre_render_update();
            m_scene_renderer->render_graph(scene, render_camera);

            m_window->swapBuffers();
            m_window->pollEvents();
        }
    }
}

#include "Engine.h"
#include "core/platform/desktop/GlfwWindow.h"

namespace yage
{
    Engine::Engine(std::unique_ptr<Application> application, int width, int height, const std::string& title)
        : m_application(std::move(application)),
          m_window(std::make_unique<platform::desktop::GlfwWindow>(width, height, title)),
          m_physics(physics3d::Visualizer())
    {
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
                        m_physics.update_staggered(dt);
                    } else {
                        m_physics.update(dt);
                    }
                    accumulator -= dt;
                }
            }

            m_application->pre_render_update();
            m_scene_renderer->render_graph(scene);

            m_window->swapBuffers();
            m_window->pollEvents();
        }
    }
}

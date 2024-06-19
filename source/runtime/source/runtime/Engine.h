#pragma once

#include "Application.h"
#include "GameObject.h"
#include "core/platform/Window.h"
#include "gl3d/sceneRenderer.h"
#include "physics3d/Simulation.h"

namespace yage
{
    class Engine
    {
    public:
        bool enable_physics_simulation = true;
        bool enable_physics_visualization = false;

        gl3d::Camera render_camera;
        physics3d::Simulation physics;
        std::shared_ptr<gl3d::SceneGroup> scene;

        Engine(int width, int height, const std::string& title);

        Engine(const Engine& other) = delete;

        Engine(Engine&& other) noexcept = delete;

        Engine& operator=(const Engine& other) = delete;

        Engine& operator=(Engine&& other) noexcept = delete;

        void run();

        template<typename App>
        void register_application()
        {
            m_application = std::make_unique<App>();
            m_application->m_engine = this;
        }

        GameObject register_game_object(const std::string& id)
        {
            m_game_objects.insert({id, GameObject()});
            return m_game_objects[id];
        }

        gl::IContext& gl_context()
        {
            return *m_gl_context;
        }

    private:
        std::unique_ptr<Application> m_application;
        std::shared_ptr<platform::IWindow> m_window;
        std::shared_ptr<gl::IContext> m_gl_context;
        std::unique_ptr<gl3d::SceneRenderer> m_scene_renderer;

        std::unordered_map<std::string, GameObject> m_game_objects;
    };
}

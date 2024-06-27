#pragma once

#include "Application.h"
#include "GameObject.h"
#include "core/platform/Window.h"
#include "gl3d/sceneRenderer.h"
#include "gl3d/MeshLoader.h"
#include "physics3d/Simulation.h"
#include "resource/Store.h"

namespace yage
{
    class Engine
    {
    private:
        std::shared_ptr<gl::IContext> m_gl_context; // this must be destructed last, so keep it at the top

    public:
        bool enable_physics_simulation = true;
        bool enable_physics_visualization = false;

        std::unique_ptr<gl3d::MeshFileLoader> mesh_loader;
        res::Store<std::unique_ptr<gl3d::Mesh>> mesh_store;

        physics3d::Simulation physics;
        std::unique_ptr<gl3d::SceneRenderer> scene_renderer;

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

        GameObject& register_game_object(const std::string& id);

        gl::IContext& gl_context();

    private:
        std::unique_ptr<Application> m_application;
        std::shared_ptr<platform::IWindow> m_window;

        std::unordered_map<std::string, GameObject> m_game_objects;
    };
}

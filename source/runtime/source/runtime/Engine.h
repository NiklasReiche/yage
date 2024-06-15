#pragma once

#include "Application.h"
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

        explicit Engine(std::unique_ptr<Application> application, int width, int height, const std::string& title = "yage");

        void run();

    private:
        std::unique_ptr<Application> m_application;
        std::unique_ptr<platform::IWindow> m_window;
        std::unique_ptr<gl3d::SceneRenderer> m_scene_renderer;
        physics3d::Simulation m_physics;
        std::shared_ptr<gl3d::SceneNode> scene;
    };
}

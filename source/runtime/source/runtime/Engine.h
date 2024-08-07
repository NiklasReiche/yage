#pragma once

#include <core/platform/Window.h>
#include <gui/master.h>
#include <gl3d/sceneRenderer.h>
#include <physics3d/Simulation.h>
#include <resource/Store.h>

#include "Application.h"
#include "GameObject.h"

namespace yage
{
    class Engine
    {
    private:
        // these must be destructed last, so keep them at the top
        std::shared_ptr<platform::IWindow> m_window;
        std::shared_ptr<gl::IContext> m_gl_context;

    public:
        bool enable_physics_simulation = true;
        bool enable_physics_visualization = false;

        gl3d::SceneRenderer scene_renderer;
        physics3d::Simulation physics;
        gui::Master gui;

        res::Store<gl3d::Mesh> mesh_store;
        res::Store<gl3d::SceneGroup> scene_store;
        res::Store<font::Font> font_store;

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

        void register_input_listener(input::InputListener& input_listener);

        GameObject& register_game_object(const std::string& id);

        void destroy_game_object(const GameObject& game_object);

        void toggle_cursor_visibility();

        gl::IContext& gl_context();

        std::string select_file();

    private:
        std::unique_ptr<Application> m_application;

        std::unordered_map<std::string, GameObject> m_game_objects;
    };
}

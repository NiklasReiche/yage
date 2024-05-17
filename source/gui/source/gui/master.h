#pragma once

#include <vector>
#include <memory>

#include <core/platform/Window.h>

#include "renderer.h"
#include "inputmanager.h"
#include "texturemanager.h"
#include "widgets/widget.h"
#include "widgets/root.h"
#include "animation.h"

namespace gui
{
    /**
     * Provides an interface for the individual components of the gui system.
     */
    class Master
    {
    public:
        Master(const std::shared_ptr<platform::IWindow>& window, const std::shared_ptr<gl::IContext>& glContext);

        /**
         * Creates a widget as a child of the UI tree's root.
         * @tparam Element The widget class to instantiate.
         * @tparam Args Constructor parameter types for the new widget.
         * @param args Constructor arguments for the new widget.
         * @return Observer pointer to the new widget.
         */
        template<typename Element, typename... Args>
        Element* create_widget(Args... args)
        {
            return m_root.create_widget<Element>(args...);
        }

        /**
         * Sets an animation as active. The animation will start being updated in the next update pass.
         */
        void activate_animation(Animation* animation);

        /**
         * Sets an animation as inactive. The animation will stop being updated in the next update pass.
         */
        void deactivate_animation(Animation* animation);

        /**
         * Performs an update pass for all active animations, using a given time step.
         * @param dt The delta time value with which to update animations.
         */
        void update(double dt);

        /**
         * Performs a rendering pass for the gui elements.
         */
        void render();

        TextureAtlasStore& texture_atlas_store()
        {
            return m_texture_manager;
        }

        InputManager& input_manager()
        {
            return m_input_manager;
        }

        gl::IContext& gl_context()
        {
            return *m_gl_context;
        }

        platform::IWindow& window()
        {
            return *m_window;
        }

        Widget& root()
        {
            return m_root;
        }

    private:
        std::shared_ptr<platform::IWindow> m_window;
        std::shared_ptr<gl::IContext> m_gl_context;

        TextureAtlasStore m_texture_manager;
        InputManager m_input_manager;
        GuiRenderer m_renderer;

        RootWidget m_root;
        std::vector<Animation*> m_animations;
        std::vector<Animation*> m_finished_animations;
    };
}
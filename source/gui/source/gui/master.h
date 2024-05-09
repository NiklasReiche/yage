#pragma once

#include <vector>
#include <memory>

#include <core/platform/Window.h>

#include "core.h"
#include "renderer.h"
#include "inputmanager.h"
#include "texturemanager.h"
#include "widget.h"
#include "root.h"
#include "animation.h"

namespace gui
{
    class Master
    {
    public:
        Master(const std::shared_ptr<platform::IWindow>& window, const std::shared_ptr<gl::IContext>& glContext);

        void update(double dt);

        void activateAnimation(Animation* animation);

        void deactivateAnimation(Animation* animation);

        template<typename Element, typename... Args>
        Element* createWidget(Widget* parent, Args... args)
        {
            if (parent == nullptr) {
                return m_root.create_widget<Element>(this, args...);
            }
            return parent->create_widget<Element>(this, args...);
        }

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
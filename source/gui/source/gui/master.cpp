#include "master.h"

namespace yage::gui
{
    Master::Master(const std::shared_ptr<platform::IWindow>& window, const std::shared_ptr<gl::IContext>& glContext)
            : m_window(window), m_gl_context(glContext),
              m_texture_manager(TextureAtlasStore(window, glContext->getTextureCreator())),
              m_input_manager(InputManager(&m_root)),
              m_renderer(GuiRenderer(glContext,
                                     gl::IRenderer::Viewport{0, 0, window->getWidth(), window->getHeight()})),
              m_root(this)
    {
        m_root.set_actual_size(math::Vec2<float>((float) window->getWidth(), (float) window->getHeight()));

        window->attach(m_input_manager);
    }

    void Master::update(double dt)
    {
        for (auto animation: m_finished_animations) {
            auto it = std::find(m_animations.begin(), m_animations.end(), animation);
            if (it != m_animations.end()) {
                m_animations.erase(it);
            }
        }
        m_finished_animations.clear();

        // copy animations, since an animation update may update the animation vector through callbacks
        std::vector<Animation*> worklist = m_animations;
        for (auto& animation: worklist) {
            animation->update(dt);
        }
    }

    void Master::render()
    {
        m_renderer.render(m_root);
    }

    void Master::activate_animation(Animation* animation)
    {
        m_animations.push_back(animation);
    }

    void Master::deactivate_animation(Animation* animation)
    {
        m_finished_animations.push_back(animation);
    }
}
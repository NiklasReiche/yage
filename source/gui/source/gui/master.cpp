#include "master.h"

namespace gui
{
	Master::Master(const std::shared_ptr<platform::IWindow> &window, const std::shared_ptr<gl::IContext> &glContext)
		: m_window(window), m_gl_context(glContext),
          m_texture_manager(TextureAtlasStore(window, glContext->getTextureCreator())),
          m_input_manager(InputManager(&m_root)),
          m_renderer(GuiRenderer(glContext->getRenderer(), glContext->getShaderCreator(), gl::IRenderer::Viewport{0, 0, window->getWidth(), window->getHeight() })),
          m_root(this)
	{
        m_root.set_actual_size(gml::Vec2<float>((float) window->getWidth(), (float) window->getHeight()));
		
		window->attach(m_input_manager);
	}

	void Master::update(double dt)
	{
		for (auto animation : m_finished_animations)
		{
            m_animations.erase(std::remove(m_animations.begin(), m_animations.end(), animation), m_animations.end());
		}
		m_finished_animations.clear();

		for (auto & animation : m_animations)
		{
			animation->update(dt);
		}

		m_renderer.render(m_root);
	}

	void Master::activateAnimation(Animation * animation)
	{
		m_animations.push_back(animation);
	}

	void Master::deactivateAnimation(Animation * animation)
	{
		m_finished_animations.push_back(animation);
	}
}
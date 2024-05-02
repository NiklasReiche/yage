#include "master.h"

namespace gui
{
	Master::Master(const std::shared_ptr<platform::IWindow> &window, const std::shared_ptr<gl::IContext> &glContext)
		: window(window), glContext(glContext),
		textureManager(TextureManager(window, glContext->getTextureCreator())),
		inputManager(InputManager(&root)),
		renderer(GuiRenderer(glContext->getRenderer(), glContext->getShaderCreator(), gl::IRenderer::Viewport{ 0, 0, window->getWidth(), window->getHeight() }))
	{
		//root.resize(gml::Vec2<float>(glContext->getWidth(), glContext->getHeight()));
		root.setSize(gml::Vec2<float>((float)window->getWidth(), (float)window->getHeight()));
		renderer.setTexture(textureManager.getTexture(0));
		
		window->attach(inputManager);
	}

	void Master::sortWidgets(std::vector<gl::IDrawable*> & vector_widget, std::vector<font::Text*> & vector_text, Widget & widget)
	{
		for (unsigned int i = 0; i < widget.getChildrenCount(); ++i)
		{
			Widget & child = widget.getChild(i);
			if (child.is_Active()) {
				vector_widget.push_back(child.drawable.get());
				if (child.has_Text()) {
					vector_text.push_back(child.getText());
				}
			}
			sortWidgets(vector_widget, vector_text, child);
		}
	}

	void Master::update(double dt)
	{
		for (unsigned int i = 0; i < finishedAnimations.size(); ++i)
		{
			Animation* animation = finishedAnimations.at(i);
			animations.erase(std::remove(animations.begin(), animations.end(), animation), animations.end());
		}
		finishedAnimations.clear();

		for (unsigned int i = 0; i < animations.size(); ++i) 
		{
			animations.at(i)->update(dt);
		}

		std::vector<gl::IDrawable*> vector_widget;
		std::vector<font::Text*> vector_text;

		sortWidgets(vector_widget, vector_text, root);

		renderer.widgets = std::move(vector_widget);
		renderer.text = std::move(vector_text);

		renderer.render();
	}

	void Master::activateAnimation(Animation * animation)
	{
		animations.push_back(animation);
	}

	void Master::deactivateAnimation(Animation * animation)
	{
		finishedAnimations.push_back(animation);
	}
}
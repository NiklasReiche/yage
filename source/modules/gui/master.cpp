#include "Master.h"

namespace gui
{
	Master::Master(platform::PlatformHandle * platform, gl::GraphicsContext * glContext, input::InputController * inputController)
		: platform(platform), glContext(glContext), 
		fontManager(FontManager(platform, glContext)),
		inputManager(InputManager(inputController->addListener(), &root)),
		renderer(GuiRenderer(platform, glContext, gl::Viewport(0, 0, glContext->getWidth(), glContext->getHeight())))
	{
		root.resize(gml::Vec2<float>(glContext->getWidth(), glContext->getHeight()));
	}

	void Master::sortWidgets(std::vector<gl::Drawable*> & vector_widget, std::vector<font::Text*> & vector_text, Widget & widget)
	{
		for (unsigned int i = 0; i < widget.getChildrenCount(); ++i)
		{
			Widget & child = widget.getChild(i);
			if (child.is_Active()) {
				vector_widget.push_back(&child);
				if (child.has_Text()) {
					vector_text.push_back(child.getText());
				}
			}
			sortWidgets(vector_widget, vector_text, child);
		}
	}

	void Master::update(double dt)
	{
		auto it = animations.begin();
		while (it != animations.end())
		{
			if ((*it)->is_finished()) {
				it = animations.erase(it);
			}
			else {
				(*it)->update(dt);
				++it;
			}
		}

		inputManager.update(root);

		std::vector<gl::Drawable*> vector_widget;
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
		animations.erase(std::remove(animations.begin(), animations.end(), animation), animations.end());
	}
}
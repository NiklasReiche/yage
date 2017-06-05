#include "master.h"

namespace gui
{
	Master::Master(ngl::Window * window)
		: window(window)
	{
		inputManger.initialize(window);
		renderer = GuiRenderer(ngl::Viewport(0, 0, window->getWidth(), window->getHeight()));
	}

	void Master::sortWidgets(std::vector<ngl::Drawable*> & vector_widget, std::vector<font::Text*> & vector_text, Widget & widget)
	{
		for (unsigned int i = 0; i < widget.getChildrenCount(); ++i)
		{
			Widget & child = widget.getChild(i);
			vector_widget.push_back(&child);
			if (child.has_Text()) {
				vector_text.push_back(child.getText());
			}
			sortWidgets(vector_widget, vector_text, child);
		}
	}

	void Master::update()
	{
		inputManger.update(root);

		std::vector<ngl::Drawable*> vector_widget;
		std::vector<font::Text*> vector_text;

		sortWidgets(vector_widget, vector_text, root);

		renderer.widgets = std::move(vector_widget);
		renderer.text = std::move(vector_text);

		renderer.render();
	}
}
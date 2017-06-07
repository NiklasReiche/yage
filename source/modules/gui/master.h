#pragma once

#include <vector>
#include <map>
#include <memory>

#include "core.h"
#include "renderer.h"
#include "fontmanager.h"
#include "input.h"
#include "interface.h"
#include "widget.h"


namespace gui
{
	class Master
	{
	private:
		gl::GraphicsContext * glContext;

		FontManager fontManager;
		InputManager inputManger;
		GuiRenderer renderer;

		Widget root;

		void sortWidgets(std::vector<gl::Drawable*> & vector_widget, std::vector<font::Text*> & vector_text, Widget & widget);
	public:
		Master(gl::GraphicsContext * glContext, input::InputController * inputController);

		void update();

		void addFont(std::string filename) { fontManager.addFont(filename); }

		template <typename Element, typename... Args>
		Element* createWidget(Widget* parent, Args... args)
		{
			ManagerInterface mInterface { &fontManager, &inputManger, &renderer };
			if (parent == nullptr) {
				return root.createWidget<Element>(mInterface, args...);
			}
			else {
				return parent->createWidget<Element>(mInterface, args...);
			}
		}
	};
}
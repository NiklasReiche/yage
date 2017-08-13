#pragma once

#include <vector>
#include <map>
#include <memory>

#include "core.h"
#include "Renderer.h"
#include "FontManager.h"
#include "InputManager.h"
#include "Interface.h"
#include "Widget.h"
#include "rootWidget.h"
#include "Animation.h"

namespace gui
{
	class Master
	{
	private:
		sys::PlatformHandle * platform;
		gl::GraphicsContext * glContext;

		FontManager fontManager;
		InputManager inputManager;
		GuiRenderer renderer;

		RootWidget root;
		std::vector<Animation*> animations;
		std::vector<Animation*> finishedAnimations;

		void sortWidgets(std::vector<gl::Drawable*> & vector_widget, std::vector<font::Text*> & vector_text, Widget & widget);

	public:
		Master(sys::PlatformHandle * platform, gl::GraphicsContext * glContext, input::InputController * inputController);

		void update(double dt);

		void addFont(std::string filename) { fontManager.addFont(filename); }

		void activateAnimation(Animation * animation);
		void deactivateAnimation(Animation * animation);

		template <typename Element, typename... Args>
		Element* createWidget(Widget* parent, Args... args)
		{
			MasterInterface master { platform, glContext, &fontManager, &inputManager, &renderer };
			if (parent == nullptr) {
				return root.createWidget<Element>(master, args...);
			}
			else {
				return parent->createWidget<Element>(master, args...);
			}
		}
	};
}
#pragma once

#include <vector>
#include <map>
#include <memory>

#include "core.h"
#include "renderer.h"
#include "fontmanager.h"
#include "inputmanager.h"
#include "texturemanager.h"
#include "interface.h"
#include "widget.h"
#include "root.h"
#include "animation.h"

namespace gui
{
	class Master
	{
	private:
		sys::PlatformHandle * platform;
		gl::GraphicsContext * glContext;

		FontManager fontManager;
		TextureManager textureManager;
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
			MasterInterface master { platform, glContext, &fontManager, &textureManager, &inputManager, &renderer };
			if (parent == nullptr) {
				return root.createWidget<Element>(master, args...);
			}
			else {
				return parent->createWidget<Element>(master, args...);
			}
		}

		MasterInterface getInterface() { return MasterInterface{ platform, glContext, &fontManager, &textureManager, &inputManager, &renderer }; }
	};
}
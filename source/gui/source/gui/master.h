#pragma once

#include <vector>
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
	public:
		Master(sys::Window window, gl::Context glContext);

		void update(double dt);

		void addFont(std::string filename) { fontManager.addFont(filename); }

		void activateAnimation(Animation * animation);
		void deactivateAnimation(Animation * animation);

		template <typename Element, typename... Args>
		Element* createWidget(Widget* parent, Args... args)
		{
			MasterInterface master { window, glContext, &fontManager, &textureManager, &inputManager, &renderer };
			if (parent == nullptr) {
				return root.createWidget<Element>(master, args...);
			}
			return parent->createWidget<Element>(master, args...);
		}

		MasterInterface getInterface() { return MasterInterface{ window, glContext, &fontManager, &textureManager, &inputManager, &renderer }; }

	private:
		sys::Window window;
		gl::Context glContext;

		FontManager fontManager;
		TextureManager textureManager;
		InputManager inputManager;
		GuiRenderer renderer;

		RootWidget root;
		std::vector<Animation*> animations;
		std::vector<Animation*> finishedAnimations;

		void sortWidgets(std::vector<gl::Drawable>& vector_widget, std::vector<font::Text*>& vector_text, Widget& widget);
	};
}
#pragma once

#include <vector>
#include <memory>

#include <core/platform/Window.h>

#include "core.h"
#include "renderer.h"
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
		Master(const std::shared_ptr<platform::IWindow> &window, const std::shared_ptr<gl::IContext> &glContext);

		void update(double dt);

		void activateAnimation(Animation * animation);
		void deactivateAnimation(Animation * animation);

		template <typename Element, typename... Args>
		Element* createWidget(Widget* parent, Args... args)
		{
			MasterInterface master { window, glContext, &textureManager, &inputManager, &renderer };
			if (parent == nullptr) {
				return root.createWidget<Element>(master, args...);
			}
			return parent->createWidget<Element>(master, args...);
		}

		MasterInterface getInterface() { return MasterInterface{ window, glContext, &textureManager, &inputManager, &renderer }; }

	private:
		std::shared_ptr<platform::IWindow> window;
		std::shared_ptr<gl::IContext> glContext;

		TextureManager textureManager;
		InputManager inputManager;
		GuiRenderer renderer;

		RootWidget root;
		std::vector<Animation*> animations;
		std::vector<Animation*> finishedAnimations;

		void sortWidgets(std::vector<gl::IDrawable*>& vector_widget, std::vector<font::Text*>& vector_text, Widget& widget);
	};
}
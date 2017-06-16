#pragma once

#include "core.h"
#include "Widget.h"

namespace gui
{
	class InputManager
	{
	private:
		input::InputListener * inputListener;
		input::InputState* input;

		bool isHoveredOver(Widget & widget);

	public:
		InputManager(input::InputListener * inputListener);

		void update(Widget & root);
		void walkWidgets(Widget & widget, int level);

		void onMouseEvent(input::MouseKeyCode, input::KeyAction);
	};
}
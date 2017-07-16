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

		Widget* rootWidget;
		Widget* selectedWidget = nullptr;
		Widget* activeWidget = nullptr;

		Widget* searchSelected(Widget * widget, float xpos, float ypos);

	public:
		InputManager(input::InputListener * inputListener, Widget* rootWidget);

		void update(Widget & root);

		void onMousePosEvent(float x, float y);
		void onMouseEvent(input::MouseKeyCode, input::KeyAction);
		void onTouchEvent(float x, float y, input::TouchIndexCode, input::TouchAction);
	};
}
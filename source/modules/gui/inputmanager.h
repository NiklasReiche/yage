#pragma once

#include <input/InputListener.h>
#include "widget.h"

namespace gui
{
	class InputManager final : public input::InputListener
	{
	public:
		explicit InputManager(Widget* rootWidget);

		void onKeyEvent(const input::KeyEvent& event) override;
		void onMousePosEvent(const input::MousePosEvent& event) override;

		void unFocus();
		
	private:
		Widget* rootWidget;
		Widget* selectedWidget = nullptr;
		Widget* activeWidget = nullptr;
		Widget* focusedWidget = nullptr;
		bool blockHover = false;

		static Widget* searchSelected(Widget * widget, float xPos, float yPos);

		void onMouseLeftPress();
		void onMouseLeftRelease();

		void onCharEvent(char character);
#if 0
		void onTouchEvent(float x, float y, input::TouchIndexCode, input::TouchAction);
#endif
	};
}

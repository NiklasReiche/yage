#pragma once

#include <core/input/InputListener.h>

#include "widgets/widget.h"

namespace gui
{
	class InputManager final : public input::InputListener
	{
	public:
		explicit InputManager(Widget* rootWidget);

		void onKeyEvent(const input::KeyEvent& event) override;
		void onMousePosEvent(const input::MousePosEvent& event) override;

        void onCharEvent(const input::CharEvent& event) override;

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


#if 0
		void onTouchEvent(float x, float y, input::TouchIndexCode, input::TouchAction);
#endif
	};
}

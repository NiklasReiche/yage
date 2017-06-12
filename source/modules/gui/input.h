#pragma once

#include "core.h"
#include "widget.h"


namespace gui
{
	enum class InputAction
	{
		IDLE,
		HOLD,
		PRESS,
		RELEASE
	};

	class InputManager
	{
	private:
		input::InputListener * inputListener;
		input::InputState* input;

		bool isHoveredOver(Widget & widget);
	public:
		InputManager() {}
		void initialize(input::InputController * inputController);

		void update(Widget & root);
		void walkWidgets(Widget & widget, int level);

		void onMouseEvent(input::MouseKeyCode, input::KeyAction);
	};
}
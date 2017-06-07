#pragma once

#include <functional>

#include <platform\Platform.h>
#include "InputState.h"

namespace input
{
	class InputController
	{
	private:
		platform::PlatformHandle* systemHandle;
		InputState input;

	public:
		InputController(platform::PlatformHandle* systemHandle);

		void poll();
		InputState* getInput() { return &input; }

		void onKeyEvent(platform::KeyCode key, platform::KeyAction action);

		void onMousePosEvent(double xpos, double ypos);
		void onMouseWheelEvent(double xoffset, double yoffset);
		void onMouseButtonEvent(platform::MouseKeyCode key, platform::KeyAction action);

		void onTouchEvent(double xpos, double ypos);
	};
}
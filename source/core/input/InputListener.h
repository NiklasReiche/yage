#pragma once

#include <platform/Platform.h>

#include "InputState.h"

namespace input
{
	class InputController;

	class InputListener
	{
	private:
		friend class InputController;

		InputController* controller;
		int id;

		std::function<void(KeyCode, KeyAction)> onKeyEventCallback;
		std::function<void(float, float)> onMousePosEventCallback;
		std::function<void(MouseKeyCode, KeyAction)> onMouseButtonEventCallback;
		std::function<void(float, float)> onMouseWheelEventCallback;
		std::function<void(float, float, TouchIndexCode, TouchAction)> onTouchEventCallback;

	public:
		InputListener() {}
		InputListener(int id);

		void setOnKeyEvent(std::function<void(KeyCode, KeyAction)> callback) { onKeyEventCallback = callback; }
		void setOnMousePosEvent(std::function<void(float, float)> callback) { onMousePosEventCallback = callback; }
		void setOnMouseButtonEvent(std::function<void(MouseKeyCode, KeyAction)> callback) { onMouseButtonEventCallback = callback; }
		void setOnMouseWheelEvent(std::function<void(float, float)> callback) { onMouseWheelEventCallback = callback; }
		void setOnTouchEvent(std::function<void(float, float, TouchIndexCode, TouchAction)> callback) { onTouchEventCallback = callback; }

		InputState* getInput();
	};
}
#pragma once

#include <functional>

#include "desktop\Keyboard.h"

namespace platform
{
	class GenericPlatformHandle
	{
	public:
		GenericPlatformHandle() {}
		virtual ~GenericPlatformHandle() {}

		virtual void poll() {}
		virtual void swapBuffer() {}

		virtual void setOnKeyEvent(std::function<void(glfw::KeyCode, glfw::KeyAction)> callback) {}
		virtual void setOnMousePosEvent(std::function<void(double, double)> callback) {}
		virtual void setOnMouseButtonEvent(std::function<void(glfw::MouseKeyCode, glfw::KeyAction)> callback) {}
		virtual void setOnMouseWheelEvent(std::function<void(double, double)> callback) {}
		virtual void setOnTouchEvent(std::function<void(double, double)>) {}
	};
}
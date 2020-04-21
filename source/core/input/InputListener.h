#pragma once

#include "KeyEvent.h"
#include "MousePosEvent.h"
#include "MouseWheelEvent.h"

namespace input
{
	class InputListener
	{
	public:
		InputListener() = default;
		virtual ~InputListener() = default;
		InputListener(const InputListener& other) = default;
		InputListener(InputListener&& other) = default;
		InputListener& operator=(const InputListener& other) = default;
		InputListener& operator=(InputListener&& other) = default;

		virtual void onKeyEvent(const KeyEvent & event) {}
		virtual void onMousePosEvent(const MousePosEvent& event) {}
		virtual void onMouseWheelEvent(const MouseWheelEvent& event) {}
	};
}

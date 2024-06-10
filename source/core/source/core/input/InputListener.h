#pragma once

#include "KeyEvent.h"
#include "MousePosEvent.h"
#include "MouseWheelEvent.h"
#include "CharEvent.h"

namespace yage::input
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

		virtual void onKeyEvent(const KeyEvent&) {}
		virtual void onMousePosEvent(const MousePosEvent&) {}
		virtual void onMouseWheelEvent(const MouseWheelEvent&) {}

        virtual void onCharEvent(const CharEvent&)
        {
        }
	};
}

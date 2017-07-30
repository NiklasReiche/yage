#pragma once

#include <functional>
#include <map>
#include <vector>

#include <platform/Platform.h>
#include "InputState.h"
#include "InputNames.h"
#include "InputListener.h"

namespace input
{
	class InputController
	{
	private:
		platform::PlatformHandle* systemHandle;

		InputState input;
		std::vector<std::function<void()>> funcs;

		int listenerId = 0;
		std::map<int, InputListener> listeners;

	public:
		InputController(platform::PlatformHandle* systemHandle);

		void poll();
		InputState* getInput() { return &input; }

		InputListener* addListener();
		void removeListener(InputListener* listener);

		void onKeyCharEvent(unsigned int codepoint, int mods);
		void onKeyEvent(int key, int action);
		void onMousePosEvent(float xpos, float ypos);
		void onMouseWheelEvent(float xoffset, float yoffset);
		void onMouseButtonEvent(int key, int action);

		//void onCharTouchEvent();
		void onTouchEvent(float xpos, float ypos, int index, int action);
	};
}
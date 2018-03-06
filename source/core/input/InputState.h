#pragma once

#include <map>

#include <math/vector.h>

#include "InputNames.h"

namespace input
{
	class InputState
	{
	private:
		friend class InputController;

		gml::Vec2<double> mousePos;
		gml::Vec2<double> mouseWheelOffset;
		std::map<KeyCode, KeyState> keyboardKeys;
		std::map<MouseKeyCode, KeyState> mouseKeys;

		std::map<TouchIndexCode, gml::Vec2f> touchPos;
		std::map<TouchIndexCode, KeyState> touchAction;

		// ToDo: joystick axis
		std::map<JoystickKeyCode, KeyState> joystickKeys;

	public:
		gml::Vec2<double> getMousePos() { return mousePos; }
		gml::Vec2<double> getMouseWheelOffset() { return mouseWheelOffset; }

		KeyState getKey(KeyCode key)
		{
			std::map<KeyCode, KeyState>::iterator it = keyboardKeys.find(key);
			if (it == std::end(keyboardKeys))
			{
				keyboardKeys[key] = KeyState::UP;
				return KeyState::UP;
			}
			else {
				return it->second;
			}
		}

		KeyState getMouseKey(MouseKeyCode key)
		{
			std::map<MouseKeyCode, KeyState>::iterator it = mouseKeys.find(key);
			if (it == std::end(mouseKeys))
			{
				mouseKeys[key] = KeyState::UP;
				return KeyState::UP;
			}
			else {
				return it->second;
			}
		}

		KeyState getJoystickKey(JoystickKeyCode key)
		{
			std::map<JoystickKeyCode, KeyState>::iterator it = joystickKeys.find(key);
			if (it == std::end(joystickKeys))
			{
				joystickKeys[key] = KeyState::UP;
				return KeyState::UP;
			}
			else {
				return it->second;
			}
		}

		KeyState getTouchState(TouchIndexCode index)
		{
			std::map<TouchIndexCode, KeyState>::iterator it = touchAction.find(index);
			if (it == std::end(touchAction))
			{
				touchAction[index] = KeyState::UP;
				return KeyState::UP;
			}
			else {
				return it->second;
			}
		}

		gml::Vec2f getTouchPos(TouchIndexCode index)
		{
			std::map<TouchIndexCode, gml::Vec2f>::iterator it = touchPos.find(index);
			if (it == std::end(touchPos))
			{
				touchPos[index] = gml::Vec2f(0.0f);
				return touchPos.at(index);
			}
			else {
				return it->second;
			}
		}
	};
}

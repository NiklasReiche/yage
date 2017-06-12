#pragma once

#include <map>

#include <math\MVector.h>

#include "InputNames.h"

namespace input
{
	class InputState
	{
	private:
		friend class InputController;

		gml::Vec2<double> mousePos;
		gml::Vec2<double> mouseWheelOffset;
		std::map<TouchIndexCode, gml::Vec2<float>> touchPos;

		std::map<KeyCode, KeyState> keyboardKeys;
		std::map<KeyCode, KeyState> keyboardKeysLast;

		std::map<MouseKeyCode, KeyState> mouseKeys;
		std::map<MouseKeyCode, KeyState> mouseKeysLast;

		// ToDo: joystick axis
		std::map<JoystickKeyCode, KeyState> joystickKeys;
		std::map<JoystickKeyCode, KeyState> joystickKeysLast;

		std::map<TouchIndexCode, KeyState> touchAction;
		std::map<TouchIndexCode, KeyState> touchActionLast;

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
		KeyState getKeyLast(KeyCode key)
		{
			std::map<KeyCode, KeyState>::iterator it = keyboardKeysLast.find(key);
			if (it == std::end(keyboardKeysLast))
			{
				keyboardKeysLast[key] = KeyState::UP;
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
		KeyState getMouseKeyLast(MouseKeyCode key)
		{
			std::map<MouseKeyCode, KeyState>::iterator it = mouseKeysLast.find(key);
			if (it == std::end(mouseKeysLast))
			{
				mouseKeysLast[key] = KeyState::UP;
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
		KeyState getJoystickKeyLast(JoystickKeyCode key)
		{
			std::map<JoystickKeyCode, KeyState>::iterator it = joystickKeysLast.find(key);
			if (it == std::end(joystickKeysLast))
			{
				joystickKeysLast[key] = KeyState::UP;
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
		KeyState getTouchStateLast(TouchIndexCode index)
		{
			std::map<TouchIndexCode, KeyState>::iterator it = touchActionLast.find(index);
			if (it == std::end(touchActionLast))
			{
				touchActionLast[index] = KeyState::UP;
				return KeyState::UP;
			}
			else {
				return it->second;
			}
		}

		gml::Vec2<float> getTouchPos(TouchIndexCode index)
		{
			std::map<TouchIndexCode, gml::Vec2<float>>::iterator it = touchPos.find(index);
			if (it == std::end(touchPos))
			{
				touchPos[index] = gml::Vec2<float>(0);
				return gml::Vec2<float>(0);
			}
			else {
				return it->second;
			}
		}
	};
}
#pragma once

#include <map>
#include <array>

#include <math\MVector.h>
#include <platform\Platform.h>

namespace input
{
	typedef platform::KeyCode KeyCode;
	typedef platform::MouseKeyCode MouseKeyCode;
	typedef platform::JoystickKeyCode JoystickKeyCode;

	enum class KeyState
	{
		IDLE,
		HOLD,
		PRESS,
		RELEASE
	};

	class InputState
	{
	private:
		friend class InputController;

		std::map<KeyCode, KeyState> keyboardKeys;

		gml::Vec2<double> mousePos;
		gml::Vec2<double> mouseWheelOffset;
		std::map<MouseKeyCode, KeyState> mouseKeys;

		// ToDo: joystick axis
		std::map<JoystickKeyCode, KeyState> joystickKeys;

		std::array<gml::Vec2<double>, 5> touch_pos;

	public:
		gml::Vec2<double> getMousePos() { return mousePos; }
		gml::Vec2<double> getMouseWheelOffset() { return mouseWheelOffset; }

		KeyState getKeyboardKey(KeyCode key)
		{
			std::map<KeyCode, KeyState>::iterator it = keyboardKeys.find(key);
			if (it == std::end(keyboardKeys))
			{
				keyboardKeys[key] = KeyState::IDLE;
				return KeyState::IDLE;
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
				mouseKeys[key] = KeyState::IDLE;
				return KeyState::IDLE;
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
				joystickKeys[key] = KeyState::IDLE;
				return KeyState::IDLE;
			}
			else {
				return it->second;
			}
		}
	};
}
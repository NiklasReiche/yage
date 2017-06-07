#include "InputController.h"

namespace input
{
	InputController::InputController(platform::PlatformHandle* systemHandle)
		: systemHandle(systemHandle)
	{
		systemHandle->setOnKeyEvent(std::bind(&InputController::onKeyEvent, this, std::placeholders::_1, std::placeholders::_2));
		systemHandle->setOnMouseButtonEvent(std::bind(&InputController::onMouseButtonEvent, this, std::placeholders::_1, std::placeholders::_2));
		systemHandle->setOnMousePosEvent(std::bind(&InputController::onMousePosEvent, this, std::placeholders::_1, std::placeholders::_2));
		systemHandle->setOnMouseWheelEvent(std::bind(&InputController::onMouseWheelEvent, this, std::placeholders::_1, std::placeholders::_2));
	}

	void InputController::poll()
	{
		systemHandle->pollEvents();
	}

	void InputController::onKeyEvent(platform::KeyCode key, platform::KeyAction action)
	{
		std::map<platform::KeyCode, KeyState>::iterator it = input.keyboardKeys.find(key);

		if (it == std::end(input.keyboardKeys))
		{
			if (action == platform::KeyAction::PRESS) {
				input.keyboardKeys[key] = KeyState::PRESS;
			}
			else if (action == platform::KeyAction::RELEASE) {
				input.keyboardKeys[key] = KeyState::RELEASE;
			}
		}
		else {
			KeyState keyState = it->second;

			if ((keyState == KeyState::RELEASE || keyState == KeyState::IDLE) && action == platform::KeyAction::PRESS) {
				input.keyboardKeys.at(key) = KeyState::PRESS;
			}
			else if (keyState == KeyState::PRESS && action == platform::KeyAction::PRESS) {
				input.keyboardKeys.at(key) = KeyState::HOLD;
			}
			else if ((keyState == KeyState::PRESS || keyState == KeyState::HOLD) && action == platform::KeyAction::RELEASE) {
				input.keyboardKeys.at(key) = KeyState::RELEASE;
			}
			else if (keyState == KeyState::RELEASE && action == platform::KeyAction::RELEASE) {
				input.keyboardKeys.at(key) = KeyState::IDLE;
			}
		}
	}

	void InputController::onMousePosEvent(double xpos, double ypos)
	{
		input.mousePos.x = xpos;
		input.mousePos.y = ypos;
	}

	void InputController::onMouseWheelEvent(double xoffset, double yoffset)
	{
		input.mouseWheelOffset.x = xoffset;
		input.mouseWheelOffset.y = yoffset;
	}

	void InputController::onMouseButtonEvent(platform::MouseKeyCode key, platform::KeyAction action)
	{
		std::map<platform::MouseKeyCode, KeyState>::iterator it = input.mouseKeys.find(key);

		if (it == std::end(input.mouseKeys))
		{
			if (action == platform::KeyAction::PRESS) {
				input.mouseKeys[key] = KeyState::PRESS;
			}
			else if (action == platform::KeyAction::RELEASE) {
				input.mouseKeys[key] = KeyState::RELEASE;
			}
		}
		else {
			KeyState keyState = it->second;

			if ((keyState == KeyState::RELEASE || keyState == KeyState::IDLE) && action == platform::KeyAction::PRESS) {
				input.mouseKeys.at(key) = KeyState::PRESS;
			}
			else if (keyState == KeyState::PRESS && action == platform::KeyAction::PRESS) {
				input.mouseKeys.at(key) = KeyState::HOLD;
			}
			else if ((keyState == KeyState::PRESS || keyState == KeyState::HOLD) && action == platform::KeyAction::RELEASE) {
				input.mouseKeys.at(key) = KeyState::RELEASE;
			}
			else if (keyState == KeyState::RELEASE && action == platform::KeyAction::RELEASE) {
				input.mouseKeys.at(key) = KeyState::IDLE;
			}
		}
	}

	void InputController::onTouchEvent(double xpos, double ypos)
	{

	}
}
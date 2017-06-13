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
		systemHandle->setOnTouchEvent(std::bind(&InputController::onTouchEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}

	void InputController::poll()
	{
		for (auto& func : funcs) func();
		funcs.clear();
		systemHandle->pollEvents();
	}

	InputListener* InputController::addListener()
	{
		listeners[listenerId] = InputListener(listenerId);
		listeners[listenerId].controller = this;
		listenerId++;
		return &listeners.at(listenerId - 1);
	}
	void InputController::removeListener(InputListener* listener)
	{
		listeners.erase(listener->id);
	}

	void InputController::onKeyEvent(int _key, int _action)
	{
#ifdef DESKTOP
		KeyAction action = KeyAction(_action);
		KeyCode key = KeyCode(_key);

		KeyState lastState = input.getKey(key);
		KeyState newState;
		if (action == KeyAction::PRESS && lastState == KeyState::UP) {
			newState = KeyState::DOWN;
			funcs.push_back(std::bind(&InputController::onKeyEvent, this, _key, _action));
		}
		else if (action == KeyAction::PRESS && lastState == KeyState::DOWN) {
			newState = KeyState::DOWN;
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::DOWN) {
			newState = KeyState::UP;
			funcs.push_back(std::bind(&InputController::onKeyEvent, this, _key, _action));
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::UP) {
			newState = KeyState::UP;
		}

		input.keyboardKeys[key] = newState;
		input.keyboardKeysLast[key] = lastState;


		for (auto const &listener : listeners)
		{
			try {
				listener.second.onKeyEventCallback(key, action);
			}
			catch (std::bad_function_call) {}
		}
#endif
	}

	void InputController::onMousePosEvent(float xpos, float ypos)
	{
		input.mousePos.x = xpos;
		input.mousePos.y = ypos;

		for (auto const &listener : listeners)
		{
			try {
				listener.second.onMousePosEventCallback(xpos, ypos);
			}
			catch (std::bad_function_call) {}
		}
	}

	void InputController::onMouseWheelEvent(float xoffset, float yoffset)
	{
		input.mouseWheelOffset.x = xoffset;
		input.mouseWheelOffset.y = yoffset;

		for (auto const &listener : listeners)
		{
			try {
				listener.second.onMouseWheelEventCallback(xoffset, yoffset);
			}
			catch (std::bad_function_call) {}
		}
	}

	void InputController::onMouseButtonEvent(int _key, int _action)
	{
#ifdef DESKTOP
		KeyAction action = KeyAction(_action);
		MouseKeyCode key = MouseKeyCode(_key);

		KeyState lastState = input.getMouseKey(key);
		KeyState newState;
		if (action == KeyAction::PRESS && lastState == KeyState::UP) {
			newState = KeyState::DOWN;
			funcs.push_back(std::bind(&InputController::onMouseButtonEvent, this, _key, _action));
		}
		else if (action == KeyAction::PRESS && lastState == KeyState::DOWN) {
			newState = KeyState::DOWN;
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::DOWN) {
			newState = KeyState::UP;
			funcs.push_back(std::bind(&InputController::onMouseButtonEvent, this, _key, _action));
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::UP) {
			newState = KeyState::UP;
		}

		input.mouseKeys[key] = newState;
		input.mouseKeysLast[key] = lastState;


		for (auto const &listener : listeners)
		{
			try {
				listener.second.onMouseButtonEventCallback(key, action);
			}
			catch (std::bad_function_call) {}
		}
#endif
	}

	void InputController::onTouchEvent(float xpos, float ypos, int index, int _action)
	{
#ifdef ANDROID
		TouchAction action = TouchAction(action);
		TouchIndexCode key = TouchIndexCode(index);

		KeyState lastState = input.getTouchState(key);
		KeyState newState;
		if (action == TouchAction::PRESS && lastState == KeyState::UP) {
			newState = KeyState::DOWN;
			funcs.push_back(std::bind(&InputController::onTouchEvent, this, xpos, ypos, index, _action));
		}
		else if (action == TouchAction::PRESS && lastState == KeyState::DOWN) {
			newState = KeyState::DOWN;
		}
		else if (action == TouchAction::RELEASE && lastState == KeyState::DOWN) {
			newState = KeyState::UP;
			funcs.push_back(std::bind(&InputController::onTouchEvent, this, xpos, ypos, index, _action));
		}
		else if (action == TouchAction::RELEASE && lastState == KeyState::UP) {
			newState = KeyState::UP;
		}

		input.touchAction[key] = newState;
		input.touchActionLast[key] = lastState;


		for (auto const &listener : listeners)
		{
			try {
				listener.second.onTouchEventCallback(xpos, ypos, key, action);
			}
			catch (std::bad_function_call) {}
		}
#endif
	}
}
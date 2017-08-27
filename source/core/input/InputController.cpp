#include "InputController.h"

namespace input
{
	InputController::InputController(sys::PlatformHandle* systemHandle)
		: systemHandle(systemHandle)
	{
		systemHandle->setOnCharModsEvent(std::bind(&InputController::onKeyCharEvent, this, std::placeholders::_1, std::placeholders::_2));
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
		listenerId++;
		listeners[listenerId] = InputListener(listenerId);
		listeners[listenerId].controller = this;
		return &listeners.at(listenerId);
	}
	void InputController::removeListener(InputListener* listener)
	{
		listeners.erase(listener->id);
	}

	void InputController::onKeyCharEvent(unsigned int codepoint, int mods)
	{
#ifdef DESKTOP
		char character = (char)codepoint;
		
		for (auto const &listener : listeners)
		{
			if (listener.second.onCharEventCallback) {
				listener.second.onCharEventCallback(character);
			}
		}
#endif
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
			for (auto const &listener : listeners)
			{
				if (listener.second.onKeyEventCallback) {
					listener.second.onKeyEventCallback(key, action);
				}
			}
			funcs.push_back(std::bind(&InputController::onKeyEvent, this, _key, _action)); /* store function call for next frame to simulate key event */
		}
		else if (action == KeyAction::PRESS && lastState == KeyState::DOWN) {
			newState = KeyState::DOWN; /* this gets called in the simulated key event, so that both key and last key are DOWN */
		}
		else if (action == KeyAction::REPEAT) {
			for (auto const &listener : listeners)
			{
				if (listener.second.onKeyEventCallback) {
					listener.second.onKeyEventCallback(key, action);
				}
			}
			newState = KeyState::DOWN;
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::DOWN) {
			newState = KeyState::UP;
			for (auto const &listener : listeners)
			{
				if (listener.second.onKeyEventCallback) {
					listener.second.onKeyEventCallback(key, action);
				}
			}
			funcs.push_back(std::bind(&InputController::onKeyEvent, this, _key, _action));
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::UP) {
			newState = KeyState::UP;
		}

		input.keyboardKeys[key] = newState;
		input.keyboardKeysLast[key] = lastState;
#endif
	}

	void InputController::onMousePosEvent(float xpos, float ypos)
	{
		input.mousePos.x = xpos;
		input.mousePos.y = ypos;

		for (auto const &listener : listeners)
		{
			if (listener.second.onMousePosEventCallback) {
				listener.second.onMousePosEventCallback(xpos, ypos);
			}
		}
	}

	void InputController::onMouseWheelEvent(float xoffset, float yoffset)
	{
		input.mouseWheelOffset.x = xoffset;
		input.mouseWheelOffset.y = yoffset;

		for (auto const &listener : listeners)
		{
			if (listener.second.onMouseWheelEventCallback){
				listener.second.onMouseWheelEventCallback(xoffset, yoffset);
			}
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
			for (auto const &listener : listeners)
			{
				if (listener.second.onMouseButtonEventCallback) {
					listener.second.onMouseButtonEventCallback(key, action);
				}
			}
			funcs.push_back(std::bind(&InputController::onMouseButtonEvent, this, _key, _action));
		}
		else if (action == KeyAction::PRESS && lastState == KeyState::DOWN) {
			newState = KeyState::DOWN;
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::DOWN) {
			newState = KeyState::UP;
			for (auto const &listener : listeners)
			{
				if (listener.second.onMouseButtonEventCallback) {
					listener.second.onMouseButtonEventCallback(key, action);
				}
			}
			funcs.push_back(std::bind(&InputController::onMouseButtonEvent, this, _key, _action));
		}
		else if (action == KeyAction::RELEASE && lastState == KeyState::UP) {
			newState = KeyState::UP;
		}

		input.mouseKeys[key] = newState;
		input.mouseKeysLast[key] = lastState;
#endif
	}

	void InputController::onTouchEvent(float xpos, float ypos, int index, int _action)
	{
#ifdef ANDROID
		TouchAction action = TouchAction(_action);
		TouchIndexCode key = TouchIndexCode(index);

		input.touchPos[key].x = xpos;
		input.touchPos[key].y = ypos;

		if (action == TouchAction::PRESS || action == TouchAction::RELEASE) {

			KeyState lastState = input.getTouchState(key);
			KeyState newState;

			if (action == TouchAction::PRESS && lastState == KeyState::UP) {
				newState = KeyState::DOWN;
				for (auto const &listener : listeners)
				{
					try {
						listener.second.onTouchEventCallback(xpos, ypos, key, action);
					}
					catch (std::bad_function_call) {}
				}
				funcs.push_back(std::bind(&InputController::onTouchEvent, this, xpos, ypos, index, _action));
			}
			else if (action == TouchAction::PRESS && lastState == KeyState::DOWN) {
				newState = KeyState::DOWN;
			}
			else if (action == TouchAction::RELEASE && lastState == KeyState::DOWN) {
				newState = KeyState::UP;
				for (auto const &listener : listeners)
				{
					try {
						listener.second.onTouchEventCallback(xpos, ypos, key, action);
					}
					catch (std::bad_function_call) {}
				}
				funcs.push_back(std::bind(&InputController::onTouchEvent, this, xpos, ypos, index, _action));
			}
			else if (action == TouchAction::RELEASE && lastState == KeyState::UP) {
				newState = KeyState::UP;
			}

			input.touchAction[key] = newState;
			input.touchActionLast[key] = lastState;
		}
		else if (action == TouchAction::MOVE) {
			TouchAction action = TouchAction(_action);
			TouchIndexCode key = TouchIndexCode(index);

			for (auto const &listener : listeners)
			{
				try {
					listener.second.onTouchEventCallback(xpos, ypos, key, action);
				}
				catch (std::bad_function_call) {}
			}

			input.touchAction[key] = KeyState::DOWN;
			input.touchActionLast[key] = KeyState::DOWN;
		}
#endif
	}
}
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

		if (action == KeyAction::PRESS) {
			inputState.keyboardKeys[key] = KeyState::DOWN;
		}
		else if (action == KeyAction::RELEASE) {
			inputState.keyboardKeys[key] = KeyState::UP;
		}
		
		for (auto const &listener : listeners)
		{
			if (listener.second.onKeyEventCallback) {
				listener.second.onKeyEventCallback(key, action);
			}
		}
#endif
	}

	void InputController::onMousePosEvent(float xpos, float ypos)
	{
		inputState.mousePos.x = xpos;
		inputState.mousePos.y = ypos;

		for (auto const &listener : listeners)
		{
			if (listener.second.onMousePosEventCallback) {
				listener.second.onMousePosEventCallback(xpos, ypos);
			}
		}
	}

	void InputController::onMouseWheelEvent(float xoffset, float yoffset)
	{
		inputState.mouseWheelOffset.x = xoffset;
		inputState.mouseWheelOffset.y = yoffset;

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

		if (action == KeyAction::PRESS) {
			inputState.mouseKeys[key] = KeyState::DOWN;
		}
		else if (action == KeyAction::RELEASE) {
			inputState.mouseKeys[key] = KeyState::UP;
		}

		for (auto const &listener : listeners)
		{
			if (listener.second.onMouseButtonEventCallback) {
				listener.second.onMouseButtonEventCallback(key, action);
			}
		}
#endif
	}

	void InputController::onTouchEvent(float xpos, float ypos, int index, int _action)
	{
#ifdef ANDROID
		TouchAction action = TouchAction(_action);
		TouchIndexCode key = TouchIndexCode(index);

		if (action == TouchAction::PRESS) {
			inputState.touchAction[key] = KeyState::DOWN;
		}
		else if (action == TouchAction::RELEASE || action == TouchAction::CANCEL) {
			inputState.touchAction[key] = KeyState::UP;
		}

		for (auto const &listener : listeners)
		{
			if (listener.second.onTouchEventCallback) {
				listener.second.onTouchEventCallback(xpos, ypos, key, action);
			}
		}
#endif
	}
}
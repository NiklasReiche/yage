﻿#include "InputManager.h"

namespace gui
{
	InputManager::InputManager(input::InputListener * inputListener, Widget* rootWidget)
		: inputListener(inputListener), rootWidget(rootWidget)
	{
		inputListener->setOnKeyEvent(std::bind(&InputManager::onKeyEvent, this, std::placeholders::_1, std::placeholders::_2));
		inputListener->setOnCharEventCallback(std::bind(&InputManager::onCharEvent, this, std::placeholders::_1));
		inputListener->setOnMousePosEvent(std::bind(&InputManager::onMousePosEvent, this, std::placeholders::_1, std::placeholders::_2));
		inputListener->setOnMouseButtonEvent(std::bind(&InputManager::onMouseEvent, this, std::placeholders::_1, std::placeholders::_2));
		inputListener->setOnTouchEvent(std::bind(&InputManager::onTouchEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}

	void InputManager::update(Widget & root)
	{
		input = inputListener->getInput();
	}

	void InputManager::unFocus()
	{
		if (focusedWidget != nullptr) {
			focusedWidget->onFocusRelease();
			focusedWidget = nullptr;
		}
	}

	Widget* InputManager::searchSelected(Widget * widget, float xpos, float ypos)
	{
		if (xpos > widget->position.x && xpos < widget->position.x + widget->size.x && ypos > widget->position.y && ypos < widget->position.y + widget->size.y) {
			Widget* result = nullptr;
			for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
			{
				result = searchSelected(&widget->getChild(i), xpos, ypos);
				if (result != nullptr) {
					break;
				}
			}

			if (result == nullptr && widget->is_Active() && widget->is_Interactable()) {
				return widget;
			}
			return result;
		}
		else {
			return nullptr;
		}
	}

	void InputManager::onCharEvent(char character)
	{
		if (focusedWidget != nullptr) {
			focusedWidget->onCharInput(character);
		}
	}
	void InputManager::onKeyEvent(input::KeyCode key, input::KeyAction action)
	{
		if (focusedWidget != nullptr && (action == input::KeyAction::PRESS || action == input::KeyAction::REPEAT)) {
			focusedWidget->onKeyPress(key);
		}
	}

	void InputManager::onMousePosEvent(float x, float y) 
	{
		if (inputListener->getInput()->getMouseKey(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::UP) {

			Widget* lastSelectedWidget = selectedWidget;
			selectedWidget = searchSelected(rootWidget, x, y);

			if (selectedWidget != lastSelectedWidget) {
				if (selectedWidget != nullptr) {
					selectedWidget->onHover();
				}
				if (lastSelectedWidget != nullptr) {
					lastSelectedWidget->onHoverRelease();
				}
			}
		}
		else if (inputListener->getInput()->getMouseKey(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::DOWN) {

			selectedWidget = searchSelected(rootWidget, x, y);

			if (selectedWidget == activeWidget && activeWidget != nullptr) {
				activeWidget->onClick();
			}
			else if (activeWidget != nullptr) {
				activeWidget->onCancel();
			}
		}
	}
	void InputManager::onMouseEvent(input::MouseKeyCode key, input::KeyAction action)
	{
		if (selectedWidget != nullptr && key == input::MouseKeyCode::KEY_MOUSE_1 && action == input::KeyAction::PRESS) {
			activeWidget = selectedWidget;
			activeWidget->onClick();
		}
		else if (activeWidget == selectedWidget && activeWidget != nullptr && key == input::MouseKeyCode::KEY_MOUSE_1 && action == input::KeyAction::RELEASE) {
			if (activeWidget->keepFocus) {
				focusedWidget = activeWidget;
				focusedWidget->onFocus();
			}
			else if (focusedWidget != nullptr){
				focusedWidget->onFocusRelease();
				focusedWidget = nullptr;
			}
			activeWidget->onClickRelease();
			activeWidget = nullptr;
		}
		else if (selectedWidget == nullptr && focusedWidget != nullptr && key == input::MouseKeyCode::KEY_MOUSE_1 && action == input::KeyAction::RELEASE) {
			focusedWidget->onFocusRelease();
			focusedWidget = nullptr;
		}
	}

	void InputManager::onTouchEvent(float x, float y, input::TouchIndexCode index, input::TouchAction action)
	{
		selectedWidget = searchSelected(rootWidget, x, y);

		if (index == input::TouchIndexCode::TOUCH_INDEX_1 && action == input::TouchAction::MOVE) {
			if (selectedWidget == activeWidget && activeWidget != nullptr) {
				activeWidget->onClick();
			}
			else if (activeWidget != nullptr) {
				activeWidget->onCancel();
			}
		}
		else if (selectedWidget != nullptr && index == input::TouchIndexCode::TOUCH_INDEX_1 && action == input::TouchAction::PRESS) {
			activeWidget = selectedWidget;
			activeWidget->onClick();
		}
		else if (activeWidget == selectedWidget && activeWidget != nullptr && index == input::TouchIndexCode::TOUCH_INDEX_1 && action == input::TouchAction::RELEASE) {
			activeWidget->onClickRelease();
			activeWidget = nullptr;
		}
	}
}
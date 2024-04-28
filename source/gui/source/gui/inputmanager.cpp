#include "inputmanager.h"

namespace gui
{
	InputManager::InputManager(Widget* rootWidget)
		: rootWidget(rootWidget) {}

	void InputManager::onKeyEvent(const input::KeyEvent& event)
	{
		const auto key = event.getKey();
		const auto action = event.getAction();

		if (key == input::KeyEvent::Code::KEY_MOUSE_1)
		{
			if (action == input::KeyEvent::Action::PRESS)
				onMouseLeftPress();
			else if (action == input::KeyEvent::Action::RELEASE)
				onMouseLeftRelease();
		}
		else if (focusedWidget != nullptr &&
			(action == input::KeyEvent::Action::PRESS || action == input::KeyEvent::Action::REPEAT))
		{
			focusedWidget->onKeyPress(key);
		}
	}

	void InputManager::onMousePosEvent(const input::MousePosEvent& event)
	{
		Widget* lastSelectedWidget = selectedWidget;
		selectedWidget = searchSelected(rootWidget, event.getXPos(), event.getYPos());

		if (selectedWidget != lastSelectedWidget)
		{
			if (activeWidget == nullptr && blockHover == false)
			{
				if (selectedWidget != nullptr)
				{
					selectedWidget->onHover();
				}
				if (lastSelectedWidget != nullptr)
				{
					lastSelectedWidget->onHoverRelease();
				}
			}
			else if (activeWidget == selectedWidget && activeWidget != nullptr)
			{
				selectedWidget->onResume();
			}
			else if (activeWidget == lastSelectedWidget && activeWidget != nullptr)
			{
				lastSelectedWidget->onHoverRelease();
			}
		}
	}

	void InputManager::unFocus()
	{
		if (focusedWidget != nullptr)
		{
			focusedWidget->onFocusRelease();
			focusedWidget = nullptr;
		}
	}

	Widget* InputManager::searchSelected(Widget* widget, const float xPos, const float yPos)
	{
		if (xPos > widget->position.x() && xPos < widget->position.x() + widget->size.x() && yPos > widget->position.y() && yPos < widget->position.y() + widget->size.y())
		{
			Widget* result = nullptr;
			for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
			{
				result = searchSelected(&widget->getChild(i), xPos, yPos);
				if (result != nullptr)
				{
					break;
				}
			}

			if (result == nullptr && widget->is_Active() && widget->is_Interactable())
			{
				return widget;
			}
			return result;
		}
		else
		{
			return nullptr;
		}
	}

	void InputManager::onMouseLeftPress()
	{
		if (selectedWidget != nullptr)
		{
			activeWidget = selectedWidget;
			activeWidget->onClick();

			if (selectedWidget != focusedWidget)
			{
				if (focusedWidget != nullptr)
				{
					focusedWidget->onFocusRelease();
					focusedWidget = nullptr;
				}
				if (selectedWidget->keepFocus)
				{
					focusedWidget = selectedWidget;
					focusedWidget->onFocus();
				}
			}
		}
		else
		{
			blockHover = true;

			if (focusedWidget != nullptr)
			{
				focusedWidget->onFocusRelease();
				focusedWidget = nullptr;
			}
		}
	}

	void InputManager::onMouseLeftRelease()
	{
		if (selectedWidget == nullptr)
		{
			if (activeWidget != nullptr)
			{
				activeWidget->onCancel();
				activeWidget = nullptr;
			}
		}
		else
		{
			if (activeWidget == selectedWidget)
			{
				activeWidget->onClickRelease();
				activeWidget = nullptr;
			}
		}
		blockHover = false;
	}

	void InputManager::onCharEvent(char character)
	{
		if (focusedWidget != nullptr)
		{
			focusedWidget->onCharInput(character);
		}
	}

#if 0
	void InputManager::onTouchEvent(float x, float y, input::TouchIndexCode index, input::TouchAction action)
	{
		selectedWidget = searchSelected(rootWidget, x, y);

		if (index == input::TouchIndexCode::TOUCH_INDEX_1 && action == input::TouchAction::MOVE)
		{
			if (selectedWidget == activeWidget && activeWidget != nullptr)
			{
				activeWidget->onClick();
			}
			else if (activeWidget != nullptr)
			{
				activeWidget->onCancel();
			}
		}
		else if (selectedWidget != nullptr && index == input::TouchIndexCode::TOUCH_INDEX_1 && action == input::TouchAction::PRESS)
		{
			activeWidget = selectedWidget;
			activeWidget->onClick();
		}
		else if (activeWidget == selectedWidget && activeWidget != nullptr && index == input::TouchIndexCode::TOUCH_INDEX_1 && action == input::TouchAction::RELEASE)
		{
			activeWidget->onClickRelease();
			activeWidget = nullptr;
		}
	}
#endif
}
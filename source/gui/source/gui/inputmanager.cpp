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
            focusedWidget->on_key_press(key);
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
                    selectedWidget->on_hover();
				}
				if (lastSelectedWidget != nullptr)
				{
                    lastSelectedWidget->on_hover_release();
				}
			}
			else if (activeWidget == selectedWidget && activeWidget != nullptr)
			{
                selectedWidget->on_resume();
			}
			else if (activeWidget == lastSelectedWidget && activeWidget != nullptr)
			{
                lastSelectedWidget->on_hover_release();
			}
		}
	}

	void InputManager::unFocus()
	{
		if (focusedWidget != nullptr)
		{
            focusedWidget->on_focus_release();
			focusedWidget = nullptr;
		}
	}

	Widget* InputManager::searchSelected(Widget* widget, const float xPos, const float yPos)
	{
		if (xPos > widget->m_position_abs.x() && xPos < widget->m_position_abs.x() + widget->m_size_abs.x() && yPos > widget->m_position_abs.y() && yPos < widget->m_position_abs.y() + widget->m_size_abs.y())
		{
			Widget* result = nullptr;
			for (auto& child : widget->children())
			{
				result = searchSelected(child.get(), xPos, yPos);
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
            activeWidget->on_click();

			if (selectedWidget != focusedWidget)
			{
				if (focusedWidget != nullptr)
				{
                    focusedWidget->on_focus_release();
					focusedWidget = nullptr;
				}
				if (selectedWidget->m_keep_focus)
				{
					focusedWidget = selectedWidget;
                    focusedWidget->on_focus();
				}
			}
		}
		else
		{
			blockHover = true;

			if (focusedWidget != nullptr)
			{
                focusedWidget->on_focus_release();
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
                activeWidget->on_cancel();
				activeWidget = nullptr;
			}
		}
		else
		{
			if (activeWidget == selectedWidget)
			{
                activeWidget->on_click_release();
				activeWidget = nullptr;
			}
		}
		blockHover = false;
	}

	void InputManager::onCharEvent(char character)
	{
		if (focusedWidget != nullptr)
		{
            focusedWidget->on_char_input(character);
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
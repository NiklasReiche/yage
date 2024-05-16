#include "inputmanager.h"

namespace gui
{
    InputManager::InputManager(Widget* root)
            : m_root_widget(root)
    {
    }

    void InputManager::onKeyEvent(const input::KeyEvent& event)
    {
        const auto key = event.getKey();
        const auto action = event.getAction();

        if (key == input::KeyEvent::Code::KEY_MOUSE_1) {
            if (action == input::KeyEvent::Action::PRESS)
                on_mouse_left_press();
            else if (action == input::KeyEvent::Action::RELEASE)
                on_mouse_left_release();
        } else if (m_focused_widget != nullptr &&
                   (action == input::KeyEvent::Action::PRESS || action == input::KeyEvent::Action::REPEAT)) {
            m_focused_widget->on_key_press(key);
        }
    }

    void InputManager::onMousePosEvent(const input::MousePosEvent& event)
    {
        Widget* last_selected = m_selected_widget;
        m_selected_widget = search_selected(m_root_widget, {event.getXPos(), event.getYPos()});

        if (m_selected_widget == last_selected)
            return;

        if (m_active_widget == nullptr && !m_should_block_hover) {
            if (last_selected != nullptr) {
                last_selected->on_hover_release();
            }
            if (m_selected_widget != nullptr) {
                m_selected_widget->on_hover();
            }
        } else if (m_active_widget == m_selected_widget && m_active_widget != nullptr) {
            m_selected_widget->on_resume();
        } else if (m_active_widget == last_selected && m_active_widget != nullptr) {
            last_selected->on_hover_release();
        }
    }

    void InputManager::release_focus()
    {
        if (m_focused_widget != nullptr) {
            m_focused_widget->on_focus_release();
            m_focused_widget = nullptr;
        }
    }

    Widget* InputManager::search_selected(Widget* widget, gml::Vec2f pos)
    {
        if (pos.x() > widget->m_position_abs.x() && pos.x() < widget->m_position_abs.x() + widget->m_size_abs.x() &&
            pos.y() > widget->m_position_abs.y() && pos.y() < widget->m_position_abs.y() + widget->m_size_abs.y()) {

            Widget* selected_child = nullptr;
            // see if we can find any interactable child, for which we can register the event instead of the parent
            for (auto& child: widget->children()) {
                selected_child = search_selected(child.get(), pos);
                if (selected_child != nullptr) {
                    break; // no need to keep searching, since only one widget should register the event
                }
            }

            if (selected_child == nullptr && widget->is_Active() && widget->is_Interactable()) {
                return widget;
            }
            return selected_child;
        } else {
            return nullptr;
        }
    }

    void InputManager::on_mouse_left_press()
    {
        if (m_selected_widget != nullptr) {
            m_active_widget = m_selected_widget;
            m_active_widget->on_click();

            if (m_selected_widget != m_focused_widget) {
                if (m_focused_widget != nullptr) {
                    m_focused_widget->on_focus_release();
                    m_focused_widget = nullptr;
                }
                if (m_selected_widget->m_keep_focus) {
                    m_focused_widget = m_selected_widget;
                    m_focused_widget->on_focus();
                }
            }
        } else {
            m_should_block_hover = true;

            if (m_focused_widget != nullptr) {
                m_focused_widget->on_focus_release();
                m_focused_widget = nullptr;
            }
        }
    }

    void InputManager::on_mouse_left_release()
    {
        if (m_selected_widget == nullptr) {
            if (m_active_widget != nullptr) {
                m_active_widget->on_cancel();
                m_active_widget = nullptr;
            }
        } else {
            if (m_active_widget == m_selected_widget && m_active_widget != nullptr) {
                m_active_widget->on_click_release();
                m_active_widget = nullptr;
            }
        }

        m_should_block_hover = false;
    }

    void InputManager::onCharEvent(const input::CharEvent& event)
    {
        InputListener::onCharEvent(event);
        if (m_focused_widget != nullptr) {
            m_focused_widget->on_char_input(event.codepoint());
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
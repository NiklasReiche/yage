#pragma once

#include <core/input/InputListener.h>

#include "widgets/widget.h"

namespace yage::gui
{
    class InputManager final : public input::InputListener
    {
    public:
        explicit InputManager(Widget* root);

        void onKeyEvent(const input::KeyEvent& event) override;

        void onMousePosEvent(const input::MousePosEvent& event) override;

        void onCharEvent(const input::CharEvent& event) override;

        /**
         * Releases the focus on the currently focused widget if there exists any. Otherwise, does nothing.
         */
        void release_focus();

    private:
        /**
         * Root of the UI hierarchy. Used as the starting point for finding interaction targets.
         */
        Widget* m_root_widget;
        /**
         * Widget within which the cursor position falls.
         */
        Widget* m_selected_widget = nullptr;
        /**
         * Widget that was the target of a previous PRESS event for which no RELEASE has yet occurred.
         */
        Widget* m_active_widget = nullptr;
        /**
         * Widget that has the current input focus.
         */
        Widget* m_focused_widget = nullptr;
        /**
         * Tracks whether hover events are suspended (i.e. when there is an active widget)
         */
        bool m_should_block_hover = false;

        /**
         * Recursively searches for the deepest interactable widget within the given position.
         * @param widget Parent widget from which to start the search.
         * @param pos Cursor position at which to search for a hit.
         * @return The found widget or nullptr if none was found.
         */
        static Widget* search_selected(Widget* widget, math::Vec2f pos);

        void on_mouse_left_press();

        void on_mouse_left_release();

#if 0 // TODO: enable when android backend is functional again
        void onTouchEvent(float x, float y, input::TouchIndexCode, input::TouchAction);
#endif
    };
}

#pragma once

#include <utility>
#include <vector>
#include <array>
#include <memory>
#include <functional>
#include <optional>

#include "widget.h"
#include "label.h"
#include "frame.h"

namespace gui
{
    struct PushButtonTemplate
    {
        /**
         * Template for the idle button state.
         */
        WidgetTemplate base{};
        /**
         * Template for the clicked button state.
         */
        WidgetTemplate click = base;
        /**
         * Template for the hovered button state.
         */
        WidgetTemplate hover = base;
        /**
         * Callback for the button click event, fired on release.
         */
        std::function<void()> command{};
    };

    /**
     * A stateless button that can be pressed.
     */
    class PushButton : public Widget
    {
    public:
        PushButton(Widget* parent, Master* master, const PushButtonTemplate& button_template);

        ~PushButton() override = default;

        void on_click() override;

        void on_click_release() override;

        void on_hover() override;

        void on_hover_release() override;

        void on_cancel() override;

        void on_resume() override;

        /**
         * Overwrites the on-click callback with a new command.
         */
        void set_command(std::function<void()> command);

    protected:
        PushButtonTemplate m_button_template;
    };

    struct CheckButtonTemplate
    {
        /**
         * Template for the idle button state when it is not selected.
         */
        WidgetTemplate base{};
        /**
         * Template for the clicked button state when it is not selected.
         */
        WidgetTemplate base_click = base;
        /**
         * Template for the hovered button state when it is not selected.
         */
        WidgetTemplate base_hover = base;
        /**
         * Template for the idle button state when it is selected.
         */
        WidgetTemplate selected = base;
        /**
         * Template for the clicked button state when it is selected.
         */
        WidgetTemplate selected_click = selected;
        /**
         * Template for the hovered button state when it is selected.
         */
        WidgetTemplate selected_hover = selected;
        /**
         * Callback for the button click event, fired on release.
         */
        std::function<void(bool)> command{};
    };

    /**
     * A stateful button, that can be either on or off.
     */
    class CheckButton : public Widget
    {
    public:
        CheckButton(Widget* parent, Master* master, const CheckButtonTemplate& button_template,
                    bool start_active = false);

        void on_click() override;

        void on_click_release() override;

        void on_hover() override;

        void on_hover_release() override;

        void on_cancel() override;

        void on_resume() override;

        /**
         * @return Whether this button is on or off.
         */
        [[nodiscard]]
        bool selected() const;

        /**
         * Overwrites the on-click callback with a new command.
         */
        void set_command(std::function<void(bool)> command);

        /**
         * Selects this button programmatically.
         */
        void select();

        /**
         * Deselects this button programmatically.
         */
        void unselect();

    protected:
        CheckButtonTemplate m_button_template;

        bool m_selected = false;
    };

    /**
     * Represents a group of Check-Buttons where only one button can be selected at any time. Associates a value with
     * each button, which sets the radio group's state if that button is selected. When no button is selected, the group
     * can either have a default value or no value, depending on how it was setup.
     */
    template<typename T>
    class RadioGroup
    {
    public:
        /**
         * Construct an empty radio group with no default value.
         */
        explicit RadioGroup(std::function<void(T)> on_change = {})
                : m_state{}, m_default{}, m_on_change(on_change)
        {
        }

        /**
         * Constructs an empty radio group with an explicit default value.
         */
        explicit RadioGroup(T default_value, std::function<void(T)> on_change = {})
                : m_state(default_value), m_default(default_value), m_on_change(on_change)
        {
        }

        /**
         * Adds a button to this group and associates the given value with it.
         * @param button The button to add.
         * @param value The value to associate with the button.
         * @param as_default Whether this button should be set by default.
         * @return The same button as was given.
         */
        CheckButton* addButton(CheckButton* button, T value, bool as_default = false)
        {
            buttons[button] = value;

            if (as_default) {
                button->select();
                m_default = value;
                if (!m_state.has_value()) {
                    m_state = value;
                }
            }

            // TODO: commands should be implemented as listeners, or else this command can be overwritten outside this method
            button->set_command([this, value, button](bool button_state) {
                if (button_state) {
                    on_radio_button_click(button, value);
                } else {
                    this->m_state = m_default;
                }
            });

            return button;
        }

        /**
         * @return The value of the currently selected button, or the default value if no button is selected, or no
         * value if there is no default value.
         */
        std::optional<T> state()
        {
            return m_state;
        }

    private:
        std::optional<T> m_state;
        std::optional<T> m_default;
        std::map<CheckButton*, T> buttons;
        std::function<void(T)> m_on_change;

        void on_radio_button_click(CheckButton* ptr, T value)
        {
            for (auto button: buttons) {
                if (button.first != ptr) {
                    button.first->unselect();
                }
            }

            m_state = value;

            if (m_on_change) {
                m_on_change(m_state.value());
            }
        }
    };
}
#pragma once

#include <utility>
#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "../core.h"
#include "../widget.h"
#include "label.h"
#include "frame.h"
#include "HListBox.h"

namespace gui
{
    struct ButtonTemplate
    {
        WidgetTemplate base{};
        WidgetTemplate click{};
        WidgetTemplate hover{};
        std::function<void()> command{};
    };

    class PushButton : public Widget
    {
    protected:
        ButtonTemplate m_button_template;

    public:
        PushButton(Widget* parent, Master* master, const ButtonTemplate& layout);

        ~PushButton() override = default;

        void on_click() override;

        void on_click_release() override;

        void on_hover() override;

        void on_hover_release() override;

        void on_cancel() override;

        void on_resume() override;

        void setCallback(std::function<void(void)> command)
        {
            m_button_template.command = std::move(command);
        }
    };

    class CheckButton : public PushButton
    {
    protected:
        bool m_state = false;

    public:
        CheckButton(Widget* parent, Master* master, const ButtonTemplate& layout, bool activate = false);

        void on_click() override;

        void on_click_release() override;

        void on_hover() override;

        void on_hover_release() override;

        void on_cancel() override;

        [[nodiscard]] bool state() const;
    };


    template<typename T>
    class RadioGroup;

    template<typename T>
    class RadioButton : public CheckButton
    {
    protected:
        T m_value;

    public:
        RadioButton(Widget* parent, Master* master, const ButtonTemplate& layout, T value, bool isDefault = false);

        void on_click() override
        {
            CheckButton::on_click();
            if (!m_state) {
                set_template(m_button_template.click);
            }
        }

        void on_click_release() override
        {
            CheckButton::on_click_release();
            if (!m_state) {
                m_state = true;
                set_template(m_button_template.base);
            }
        }

        virtual void deactivate()
        {
            m_state = false;
            set_template(m_button_template.base);
        }

        T value() const
        {
            return m_value;
        }
    };

    template<typename T>
    RadioButton<T>::RadioButton(Widget* parent, Master* master, const ButtonTemplate& layout, T value, bool isDefault)
            : CheckButton(parent, master, layout, isDefault), m_value(value)
    {
    }

    template<typename T>
    class RadioGroup
    {
    private:
        T state;
        std::vector<RadioButton<T>*> buttons;
        std::function<void(T)> m_on_change;

    public:
        RadioGroup() : state{}
        {
        }

        explicit RadioGroup(T defaultValue) : state(defaultValue)
        {
        }

        RadioGroup(T defaultValue, std::function<void(T)> on_change) : state(defaultValue), m_on_change(on_change)
        {
        }

        RadioButton<T>* addButton(RadioButton<T>* button, bool isDefault = false)
        {
            buttons.push_back(button);
            auto index = buttons.size() - 1;
            button->setCallback([this, button, index]() {
                this->onRadioButtonClick(index, button->value());
            });
            if (isDefault) {
                state = button->value();
            }
            return button;
        }

        void onRadioButtonClick(std::size_t i, T value)
        {
            if (state != value) {
                buttons.at(i)->deactivate();
                state = value;

                if (m_on_change) {
                    m_on_change(state);
                }
            }
        }

        T getState()
        {
            return state;
        }
    };
}
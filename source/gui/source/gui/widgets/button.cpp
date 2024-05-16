#include "button.h"

namespace gui
{
    PushButton::PushButton(Widget* parent, Master* master, const PushButtonTemplate& button_template)
            : Widget(parent, master, button_template.base), m_button_template(button_template)
    {
        m_is_interactable = true;

        // TODO
        if (m_template.geometry.preferred_size.value == gml::Vec2f(0.0f)) {
            m_template.geometry.size_hint = gml::Vec2<SizeHint>(SizeHint::FIT_CHILDREN);
        }
    }

    void PushButton::on_click()
    {
        set_template(m_button_template.click);
    }

    void PushButton::on_click_release()
    {
        set_template(m_button_template.base);

        if (m_button_template.command) {
            m_button_template.command();
        }
    }

    void PushButton::on_hover()
    {
        set_template(m_button_template.hover);
    }

    void PushButton::on_hover_release()
    {
        set_template(m_button_template.base);
    }

    void PushButton::on_cancel()
    {
        set_template(m_button_template.base);
    }

    void PushButton::on_resume()
    {
        set_template(m_button_template.click);
    }

    void PushButton::set_command(std::function<void()> command)
    {
        m_button_template.command = std::move(command);
    }


    CheckButton::CheckButton(Widget* parent, Master* master, const CheckButtonTemplate& button_template,
                             bool start_active)
            : Widget(parent, master, start_active ? button_template.selected : button_template.base),
              m_button_template(button_template),
              m_selected(start_active)
    {
        m_is_interactable = true;

        // TODO
        if (m_template.geometry.preferred_size.value == gml::Vec2f(0.0f)) {
            m_template.geometry.size_hint = gml::Vec2<SizeHint>(SizeHint::FIT_CHILDREN);
        }
    }

    void CheckButton::on_click()
    {
        set_template(m_selected ? m_button_template.selected_click : m_button_template.base_click);
    }

    void CheckButton::on_click_release()
    {
        m_selected = !m_selected;

        set_template(m_selected ? m_button_template.selected : m_button_template.base);

        if (m_button_template.command) {
            m_button_template.command(m_selected);
        }
    }

    void CheckButton::on_hover()
    {
        set_template(m_selected ? m_button_template.selected_hover : m_button_template.base_hover);
    }

    void CheckButton::on_hover_release()
    {
        set_template(m_selected ? m_button_template.selected : m_button_template.base);
    }

    void CheckButton::on_cancel()
    {
        set_template(m_selected ? m_button_template.selected : m_button_template.base);
    }

    void CheckButton::on_resume()
    {
        set_template(m_selected ? m_button_template.selected_click : m_button_template.base_click);
    }

    bool CheckButton::selected() const
    {
        return m_selected;
    }

    void CheckButton::set_command(std::function<void(bool)> command)
    {
        m_button_template.command = std::move(command);
    }

    void CheckButton::select()
    {
        m_selected = true;
        set_template(m_button_template.selected);

        if (m_button_template.command) {
            m_button_template.command(m_selected);
        }
    }

    void CheckButton::unselect()
    {
        m_selected = false;
        set_template(m_button_template.base);

        if (m_button_template.command) {
            m_button_template.command(m_selected);
        }
    }
}
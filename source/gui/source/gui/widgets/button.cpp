#include "button.h"

namespace gui
{
	PushButton::PushButton(Widget * parent, Master* master, const ButtonTemplate & buttonTemplate)
		: Widget(parent, master, buttonTemplate.base), m_button_template(buttonTemplate)
	{
		this->m_is_interactable = true;

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


	CheckButton::CheckButton(Widget * parent, Master* master, const ButtonTemplate & layout, bool activate)
		: PushButton(parent, master, layout)
	{
		if (activate) {
            PushButton::set_template(m_button_template.click);
			m_state = true;
		}
	}

	void CheckButton::on_click()
	{
		if (m_state) {
            set_template(m_button_template.base);
		}
		else {
            set_template(m_button_template.click);
		}
	}

	void CheckButton::on_click_release()
	{
		m_state = !m_state;

        if (m_button_template.command){
            m_button_template.command();
        }
	}

	void CheckButton::on_hover()
	{
		if (!m_state)
		{
            set_template(m_button_template.hover);
		}
	}

	void CheckButton::on_hover_release()
	{
		if (!m_state) {
            set_template(m_button_template.base);
		}
		else {
            set_template(m_button_template.click);
		}
	}

	void CheckButton::on_cancel()
	{
		if (m_state) {
            set_template(m_button_template.click);
		}
		else {
            set_template(m_button_template.base);
		}
	}

    bool CheckButton::state() const
    {
        return m_state;
    }

}
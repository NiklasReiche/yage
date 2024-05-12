#include "button.h"

namespace gui
{
	PushButton::PushButton(Widget * parent, Master* master, const ButtonTemplate & buttonTemplate)
		: Widget(parent, master, buttonTemplate.base), command(buttonTemplate.command),
        idleTexCoords(m_texture_atlas_view),
        hoverTexCoords(load_texture(buttonTemplate.hoverTexture)),
        clickTexCoords(load_texture(buttonTemplate.clickTexture))
	{
		this->m_is_interactable = true;
		this->idleColor = buttonTemplate.base.color;
		this->hoverColor = buttonTemplate.hoverColor;
		this->clickColor = buttonTemplate.clickColor;

		LabelTemplate labelTemplate{
            .text = buttonTemplate.text
        };
		labelTemplate.base.color = 0x00000000u;
		labelTemplate.base.shadow.offset = 0;
		labelTemplate.base.border.thickness = 0;

		label = this->create_widget<Label>(master, labelTemplate);

        // TODO
		if (m_template.geometry.preferred_size.value == gml::Vec2f(0.0f)) {
			m_template.geometry.size_hint = gml::Vec2<SizeHint>(SizeHint::FIT_CHILDREN);
		}
	}

	void PushButton::on_click()
	{
        m_template.color = clickColor;
		setTextureAtlasView(clickTexCoords);
        update_vertices();
	}

	void PushButton::on_click_release()
	{
        m_template.color = idleColor;
        setTextureAtlasView(idleTexCoords);
        update_vertices();

		try {
			command();
		}
		catch (std::bad_function_call) {}
	}

	void PushButton::on_hover()
	{
        m_template.color = hoverColor;
        setTextureAtlasView(hoverTexCoords);
        update_vertices();
	}

	void PushButton::on_hover_release()
	{
        m_template.color = idleColor;
        setTextureAtlasView(idleTexCoords);
        update_vertices();
	}

	void PushButton::on_cancel()
	{
        m_template.color = idleColor;
        setTextureAtlasView(idleTexCoords);
        update_vertices();
	}

	void PushButton::on_resume()
	{
        m_template.color = clickColor;
        setTextureAtlasView(clickTexCoords);
        update_vertices();
	}

	gml::Vec2f PushButton::calcPrefSize()
	{
		return label->preferred_size();
	}


	CheckButton::CheckButton(Widget * parent, Master* master, const ButtonTemplate & layout, bool activate)
		: PushButton(parent, master, layout)
	{
		if (activate) {
            m_template.color = clickColor;
            setTextureAtlasView(clickTexCoords);
            update_vertices();
			state = true;
		}
	}

	void CheckButton::on_click()
	{
		if (state) {
            m_template.color = idleColor;
            setTextureAtlasView(idleTexCoords);
		}
		else {
            m_template.color = clickColor;
            setTextureAtlasView(clickTexCoords);
		}

        update_vertices();
	}

	void CheckButton::on_click_release()
	{
		if (state) {
			state = false;
		}
		else {
			state = true;
		}

		try {
			command();
		}
		catch (std::bad_function_call) {}
	}

	void CheckButton::on_hover()
	{
		if (!state)
		{
            m_template.color = hoverColor;
            setTextureAtlasView(hoverTexCoords);
            update_vertices();
		}
	}

	void CheckButton::on_hover_release()
	{
		if (!state) {
            m_template.color = idleColor;
            setTextureAtlasView(idleTexCoords);
		}
		else {
            m_template.color = clickColor;
            setTextureAtlasView(clickTexCoords);
		}
        update_vertices();
	}

	void CheckButton::on_cancel()
	{
		if (state) {
            m_template.color = clickColor;
            setTextureAtlasView(clickTexCoords);
		}
		else {
            m_template.color = idleColor;
            setTextureAtlasView(idleTexCoords);
		}

        update_vertices();
	}

}
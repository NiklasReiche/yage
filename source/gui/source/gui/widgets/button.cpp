#include "button.h"

namespace gui
{
	PushButton::PushButton(Widget * parent, Master* master, const ButtonTemplate & buttonTemplate)
		: Widget(parent, master, buttonTemplate), command(buttonTemplate.command),
        idleTexCoords(m_texture_atlas_view),
        hoverTexCoords(load_texture(buttonTemplate.hoverTexture)),
        clickTexCoords(load_texture(buttonTemplate.clickTexture))
	{
		this->m_is_interactable = true;
		this->idleColor = gl::toVec4(buttonTemplate.color);
		this->hoverColor = gl::toVec4(buttonTemplate.hoverColor);
		this->clickColor = gl::toVec4(buttonTemplate.clickColor);

		LabelTemplate labelTemplate{
            .text = buttonTemplate.text
        };
		labelTemplate.geometry.offset = gml::Vec2f(0.0f);
		labelTemplate.geometry.size = m_hint_pref_size;
		labelTemplate.color = 0x00000000u;
		labelTemplate.shadow.offset = 0;
		labelTemplate.border.size = 0;

		label = this->create_widget<Label>(master, labelTemplate);

		this->m_layout = std::make_unique<VListLayout>();

		if (m_hint_pref_size == gml::Vec2f(0.0f)) {
            m_fit_children = true;
            m_hint_pref_size = calcPrefSize();
			//sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
		}
	}

	void PushButton::on_click()
	{
		setColor(clickColor);
		setTextureAtlasView(clickTexCoords);
        update_vertices();
	}

	void PushButton::on_click_release()
	{
		setColor(idleColor);
        setTextureAtlasView(idleTexCoords);
        update_vertices();

		try {
			command();
		}
		catch (std::bad_function_call) {}
	}

	void PushButton::on_hover()
	{
		setColor(hoverColor);
        setTextureAtlasView(hoverTexCoords);
        update_vertices();
	}

	void PushButton::on_hover_release()
	{
		setColor(idleColor);
        setTextureAtlasView(idleTexCoords);
        update_vertices();
	}

	void PushButton::on_cancel()
	{
		setColor(idleColor);
        setTextureAtlasView(idleTexCoords);
        update_vertices();
	}

	void PushButton::on_resume()
	{
		setColor(clickColor);
        setTextureAtlasView(clickTexCoords);
        update_vertices();
	}

	gml::Vec2f PushButton::calcPrefSize()
	{
		return label->getPreferredSize();
	}


	CheckButton::CheckButton(Widget * parent, Master* master, const ButtonTemplate & layout, bool activate)
		: PushButton(parent, master, layout)
	{
		if (activate) {
			setColor(clickColor);
            setTextureAtlasView(clickTexCoords);
            update_vertices();
			state = true;
		}
	}

	void CheckButton::on_click()
	{
		if (state) {
			setColor(idleColor);
            setTextureAtlasView(idleTexCoords);
		}
		else {
			setColor(clickColor);
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
			setColor(hoverColor);
            setTextureAtlasView(hoverTexCoords);
            update_vertices();
		}
	}

	void CheckButton::on_hover_release()
	{
		if (!state) {
			setColor(idleColor);
            setTextureAtlasView(idleTexCoords);
		}
		else {
			setColor(clickColor);
            setTextureAtlasView(clickTexCoords);
		}
        update_vertices();
	}

	void CheckButton::on_cancel()
	{
		if (state) {
			setColor(clickColor);
            setTextureAtlasView(clickTexCoords);
		}
		else {
			setColor(idleColor);
            setTextureAtlasView(idleTexCoords);
		}

        update_vertices();
	}

}
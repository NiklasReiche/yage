#include "button.h"

namespace gui
{
	PushButton::PushButton(Widget * parent, Master* master, const ButtonTemplate & buttonTemplate)
		: Widget(parent, master, buttonTemplate), command(buttonTemplate.command),
        idleTexCoords(m_texture_atlas_view),
        hoverTexCoords(loadTexture(buttonTemplate.hoverTexture)),
        clickTexCoords(loadTexture(buttonTemplate.clickTexture))
	{
		this->isInteractable = true;
		this->idleColor = gl::toVec4(buttonTemplate.color);
		this->hoverColor = gl::toVec4(buttonTemplate.hoverColor);
		this->clickColor = gl::toVec4(buttonTemplate.clickColor);

		LabelTemplate labelTemplate{
            .text = buttonTemplate.text
        };
		labelTemplate.geometry.offset = gml::Vec2f(0.0f);
		labelTemplate.geometry.size = prefSize;
		labelTemplate.color = 0x00000000u;
		labelTemplate.shadow.offset = 0;
		labelTemplate.border.size = 0;

		label = this->createWidget<Label>(master, labelTemplate);

		this->m_layout = std::make_unique<VListLayout>();

		if (prefSize == gml::Vec2f(0.0f)) {
			fitChildren = true;
			prefSize = calcPrefSize();
			//sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
		}
	}

	void PushButton::onClick()
	{
		setColor(clickColor);
		setTextureAtlasView(clickTexCoords);
        update_parameters();
	}

	void PushButton::onClickRelease()
	{
		setColor(idleColor);
        setTextureAtlasView(idleTexCoords);
        update_parameters();

		try {
			command();
		}
		catch (std::bad_function_call) {}
	}

	void PushButton::onHover()
	{
		setColor(hoverColor);
        setTextureAtlasView(hoverTexCoords);
        update_parameters();
	}

	void PushButton::onHoverRelease()
	{
		setColor(idleColor);
        setTextureAtlasView(idleTexCoords);
        update_parameters();
	}

	void PushButton::onCancel() 
	{
		setColor(idleColor);
        setTextureAtlasView(idleTexCoords);
        update_parameters();
	}

	void PushButton::onResume()
	{
		setColor(clickColor);
        setTextureAtlasView(clickTexCoords);
        update_parameters();
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
            update_parameters();
			state = true;
		}
	}

	void CheckButton::onClick()
	{
		if (state) {
			setColor(idleColor);
            setTextureAtlasView(idleTexCoords);
		}
		else {
			setColor(clickColor);
            setTextureAtlasView(clickTexCoords);
		}

        update_parameters();
	}

	void CheckButton::onClickRelease()
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

	void CheckButton::onHover()
	{
		if (!state)
		{
			setColor(hoverColor);
            setTextureAtlasView(hoverTexCoords);
            update_parameters();
		}
	}

	void CheckButton::onHoverRelease()
	{
		if (!state) {
			setColor(idleColor);
            setTextureAtlasView(idleTexCoords);
		}
		else {
			setColor(clickColor);
            setTextureAtlasView(clickTexCoords);
		}
        update_parameters();
	}

	void CheckButton::onCancel()
	{
		if (state) {
			setColor(clickColor);
            setTextureAtlasView(clickTexCoords);
		}
		else {
			setColor(idleColor);
            setTextureAtlasView(idleTexCoords);
		}

        update_parameters();
	}

}
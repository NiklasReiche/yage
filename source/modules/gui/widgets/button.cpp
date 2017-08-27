#include "Button.h"

namespace gui
{
	PushButton::PushButton(Widget * parent, MasterInterface master, const ButtonTemplate & buttonTemplate)
		: Widget(parent, master, buttonTemplate), command(buttonTemplate.command)
	{
		this->isInteractable = true;
		this->idleColor = gl::toVec4(buttonTemplate.color);
		this->hoverColor = gl::toVec4(buttonTemplate.hoverColor);
		this->clickColor = gl::toVec4(buttonTemplate.clickColor);

		LabelTemplate labelTemplate;
		labelTemplate.geometry.offset = gml::Vec2f(0.0f);
		labelTemplate.geometry.size = prefSize;
		labelTemplate.color = 0x00000000u;
		labelTemplate.shadow.offset = 0;
		labelTemplate.border.size = 0;
		labelTemplate.text = buttonTemplate.text;

		label = this->createWidget<Label>(master, labelTemplate);

		this->layout = std::make_unique<VListLayout>();

		if (prefSize == gml::Vec2f(0.0f)) {
			fitChildren = true;
			prefSize = calcPrefSize();
			sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
		}
	}

	void PushButton::onClick()
	{
		setColor(clickColor);
		updateParams();
	}

	void PushButton::onClickRelease()
	{
		setColor(idleColor);
		updateParams();

		try {
			command();
		}
		catch (std::bad_function_call) {}
	}

	void PushButton::onHover()
	{
		setColor(hoverColor);
		updateParams();
	}

	void PushButton::onHoverRelease()
	{
		setColor(idleColor);
		updateParams();
	}

	void PushButton::onCancel() 
	{
		setColor(idleColor);
		updateParams();
	}

	void PushButton::onResume()
	{
		setColor(clickColor);
		updateParams();
	}

	gml::Vec2f PushButton::calcPrefSize()
	{
		return label->getPreferredSize();
	}


	CheckButton::CheckButton(Widget * parent, MasterInterface master, const ButtonTemplate & layout, bool activate)
		: PushButton(parent, master, layout)
	{
		if (activate) {
			setColor(clickColor);
			updateParams();
			state = true;
		}
	}

	void CheckButton::onClick()
	{
		if (state) {
			setColor(idleColor);
		}
		else {
			setColor(clickColor);
		}

		updateParams();
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
			updateParams();
		}
	}

	void CheckButton::onHoverRelease()
	{
		if (!state) {
			setColor(idleColor);
		}
		else {
			setColor(clickColor);
		}
		updateParams();
	}

	void CheckButton::onCancel()
	{
		if (state) {
			setColor(clickColor);
		}
		else {
			setColor(idleColor);
		}

		updateParams();
	}

}
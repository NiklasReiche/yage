#include "Button.h"

namespace gui
{
	PushButton::PushButton(Widget * parent, MasterInterface master, const ButtonTemplate & layout)
		: Widget(parent, master, layout), command(layout.command)
	{
		this->isInteractable = true;
		this->idleColor = gl::toVec4(layout.color);
		this->hoverColor = gl::toVec4(layout.hoverColor);
		this->clickColor = gl::toVec4(layout.clickColor);

		LabelTemplate labelLayout;
		labelLayout.geometry.offset = gml::Vec2f();
        labelLayout.geometry.size = gml::Vec2f();
		labelLayout.color = 0x00000000u;
		labelLayout.shadow.offset = 0;
		labelLayout.border.size = 0;
		labelLayout.text = layout.text;

		label = this->createWidget<Label>(master, labelLayout);

		this->layout = std::make_unique<VListLayout>();
		parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_AROUND);
		childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
		prefSize = this->layout->calcParentPrefSize(this) + gml::Vec2f(borderSize);
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
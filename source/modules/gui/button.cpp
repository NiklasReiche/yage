#include "button.h"


namespace gui
{
	PushButton::PushButton(Widget * parent, ManagerInterface mInterface, W_Geometry geometry, W_Border border, unsigned int color, unsigned int hoverColor)
		: Widget(parent, mInterface, geometry, color, border, W_Shadow{ 0, 0.0f })
	{
		this->primaryColor = ngl::toVec4(color);
		this->secondaryColor = ngl::toVec4(hoverColor);

		geometry.position = gml::Vec2<float>();
		color = 0x00u;
		label = this->createWidget<Label>(mInterface, geometry, color, W_Border{ 0, 0x00u }, W_Text());

		if (size == gml::Vec2<float>(0.0f)) {
			this->size = label->getSize();
			updateParams();
		}
	}

	void PushButton::onClickRelease()
	{
		command();
	}

	void PushButton::onHover()
	{
		setColor(gml::Vec4<float>(secondaryColor.x, secondaryColor.y, secondaryColor.z, secondaryColor.w));
		updateParams();
	}

	void PushButton::onHoverRelease()
	{
		setColor(gml::Vec4<float>(primaryColor.x, primaryColor.y, primaryColor.z, primaryColor.w));
		updateParams();
	}
}
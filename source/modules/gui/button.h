#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "core.h"

#include "widget.h"
#include "label.h"


namespace gui
{
	class PushButton : public Widget
	{
	private:
		Label * label;
		std::function<void()> command;
		gml::Vec4<float> primaryColor;
		gml::Vec4<float> secondaryColor;
	public:
		PushButton(Widget * parent, ManagerInterface mInterface, W_Geometry geometry, W_Border border, unsigned int color, unsigned int hoverColor);

		virtual void onClickRelease();
		virtual void onHover();
		virtual void onHoverRelease();

		void setCallback(std::function<void(void)> command) { this->command = command; }
		void setText(W_Text text) { label->setText(text); }
		void setTextColor(unsigned int color) { label->setTextColor(color); }
	};
}
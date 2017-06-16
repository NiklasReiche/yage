#pragma once

#include <string>

#include "../core.h"
#include "../Widget.h"

namespace gui
{
	struct W_Text
	{
		std::string text;
		std::string font;
		int size;
		unsigned int color;
		unsigned int bgColor;
	};

	class Label : public Widget
	{
	private:
		font::Text text;

		float padding = 2;

		gml::Vec3<float> backgroundColor;
		gml::Vec3<float> hoverColor;
	public:
		Label(Widget * parent, ManagerInterface mInterface, W_Geometry geometry, unsigned int color, W_Border border, W_Text text);

		void onHover() {}
		void onHoverRelease() {}

		font::Text* getText() { return &text; }

		void setText(W_Text text);
		void setTextColor(unsigned int color) { text.setColor(color); }
	};
}
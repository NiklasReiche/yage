#pragma once

#include <string>

#include "../core.h"
#include "../Widget.h"

namespace gui
{
	struct TextLayout
	{
		std::string text = "";
		std::string font = "arial";
		int size = 16;
		unsigned int color = gl::Color::BLACK;
	};

	class Label : public Widget
	{
	private:
		font::Text text;

		float padding = 2;


	public:
		Label(Widget * parent, MasterInterface master, WidgetLayout layout, TextLayout text);

		font::Text* getText() { return &text; }

		void setText(TextLayout text);
		void setTextColor(unsigned int color) { text.setColor(color); }

		void move(gml::Vec2<float> position);
	};
}
#pragma once

#include <string>

#include "../core.h"
#include "../Widget.h"

namespace gui
{
	struct LabelLayout : public WidgetLayout
	{
		gml::Vec2f padding = 2.0f;
	};
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

		gml::Vec2f padding = gml::Vec2f(2.0f);

	public:
		Label(Widget * parent, MasterInterface master, LabelLayout layout, TextLayout text);

		font::Text* getText() { return &text; }

		void setText(TextLayout text);
		void setTextColor(unsigned int color) { text.setColor(color); }

		void updateGeometry();
	};
}
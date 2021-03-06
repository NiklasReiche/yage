#pragma once

#include <string>

#include "../core.h"
#include "../widget.h"

namespace gui
{
	enum class TextAlignmentY
	{
		TOP,
		BOTTOM,
		CENTER
	};
	enum class TextAlignmentX
	{
		LEFT, 
		RIGHT, 
		CENTER
	};

	struct TextTemplate
	{
		std::string text = "";
		std::string font = "arial";
		int size = 16;
		unsigned int color = gl::Color::BLACK;
		TextAlignmentX alignX = TextAlignmentX::LEFT;
		TextAlignmentY alignY = TextAlignmentY::TOP;
	};
	struct LabelTemplate : public WidgetTemplate
	{
		TextTemplate text;
		gml::Vec2f padding = gml::Vec2f(2.0f);
	};

	class Label : public Widget
	{
	private:
		font::Text text;
		std::string font = "arial";

		gml::Vec2f padding = gml::Vec2f(2.0f);
		TextAlignmentX alignX = TextAlignmentX::LEFT;
		TextAlignmentY alignY = TextAlignmentY::TOP;

	public:
		Label(Widget * parent, MasterInterface master, LabelTemplate labelTemplate);

		font::Text* getText() { return &text; }

		void setText(TextTemplate text);
		void setText(font::Text text);
		void setText(std::string text);
		void setTextColor(unsigned int color) { text.setColor(color); }

		void updateGeometry();

		gml::Vec2f calcPrefSize();
	};
}
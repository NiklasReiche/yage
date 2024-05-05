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
		std::u32string text;
        res::Resource<font::Font> font;
		int size = 16;
		gl::Color_t color = gl::Color::BLACK;
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
        LabelTemplate params;
		std::unique_ptr<font::Text> text;

		gml::Vec2f padding = gml::Vec2f(2.0f);
		TextAlignmentX alignX = TextAlignmentX::LEFT;
		TextAlignmentY alignY = TextAlignmentY::TOP;

	public:
		Label(Widget * parent, Master* master, LabelTemplate labelTemplate);

		font::Text* getText() { return text.get(); } // TODO

		void setText(TextTemplate text);

		void setText(const std::u32string &string);

		void update_geometry();

		gml::Vec2f calcPrefSize();
	};
}
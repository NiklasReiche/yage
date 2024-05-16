#pragma once

#include <string>

#include "widget.h"

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
        res::Resource<font::Font> font; // TODO: provide a default font somewhere
		int size = 16;
		gl::Color_t color = gl::Color::BLACK;
		TextAlignmentX alignX = TextAlignmentX::LEFT;
		TextAlignmentY alignY = TextAlignmentY::TOP;
	};

	struct LabelTemplate
	{
        WidgetTemplate base {};
		TextTemplate text;
		gml::Vec2f padding = gml::Vec2f(4.0f); // TODO: make this part of widget base class
	};

	class Label : public Widget
	{
	private:
        LabelTemplate params;
		std::unique_ptr<font::Text> m_text;

		gml::Vec2f padding = gml::Vec2f(2.0f);
		TextAlignmentX alignX = TextAlignmentX::LEFT;
		TextAlignmentY alignY = TextAlignmentY::TOP;

	public:
		Label(Widget * parent, Master* master, LabelTemplate labelTemplate);

		font::Text* text() const override;

		void setText(TextTemplate text);

		void setText(const std::u32string &string);

		void update_geometry() override;

		gml::Vec2f preferred_size() const override;
	};
}
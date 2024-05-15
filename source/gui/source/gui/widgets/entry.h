#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "../core.h"
#include "../inputmanager.h"
#include "../widget.h"
#include "label.h"

namespace gui
{
	class TextCursor : public Widget
	{
	public:
		TextCursor(Widget* parent, Master* master, WidgetTemplate widgetTemplate);
	};

	class CursorAnimation : public Animation
	{
	public:
		CursorAnimation(Widget* widget, Master* master, double time);
		void update(double dt) override;
	};


	struct TextEntryTemplate
	{
        WidgetTemplate base;
        gml::Vec2f padding = gml::Vec2f(2.0f);
		TextTemplate defaultText;
		TextTemplate inputText;
		gl::Color_t cursorColor = gl::Color::BLACK;
		float cursorWidth = 1;
		std::function<void()> command{};
	};

	class TextEntry : public Widget
	{
	private:
		Label* label;
		TextCursor* cursor;
		CursorAnimation* cursorAnimation;

		gml::Vec2f padding = gml::Vec2f(2.0f);
		std::function<void()> callback;

		unsigned int cursorColor = gl::Color::BLACK;
		float cursorWidth = 1.0f;
		int cursorPosition = 0;

		TextTemplate defaultTextTemplate;
		TextTemplate inputTextTemplate;
		std::u32string inputText;

		void moveCursor();

	public:
		TextEntry(Widget * parent, Master* master, TextEntryTemplate entryTemplate);

		void on_focus() override;
		void on_focus_release() override;
		void on_char_input(unsigned int character) override;
		void on_key_press(input::KeyEvent::Code key) override;

        [[nodiscard]]
        std::u32string value() const;
    };
}
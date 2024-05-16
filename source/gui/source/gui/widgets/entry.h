#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "../inputmanager.h"
#include "widget.h"
#include "label.h"

namespace gui
{
	class TextCursor : public Widget
	{
	public:
		TextCursor(Widget* parent, Master* master, const WidgetTemplate& widget_template);
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
		TextTemplate placeholder_text;
		TextTemplate input_text;
		gl::Color_t cursor_color = gl::Color::BLACK;
		std::function<void()> command{};
	};

    /**
     * Represents a single line of text that listens to character input.
     */
	class TextEntry : public Widget
	{
    public:
		TextEntry(Widget * parent, Master* master, const TextEntryTemplate& entry_template);

		void on_focus() override;
		void on_focus_release() override;
		void on_char_input(unsigned int character) override;
		void on_key_press(input::KeyEvent::Code key) override;

        /**
         * @return The current text input.
         */
        [[nodiscard]]
        std::u32string value() const;

    private:
        const float CURSOR_WIDTH = 1.0f;

        Label* label;
        TextCursor* cursor;
        CursorAnimation* cursor_animation;

        TextEntryTemplate m_entry_template;

        std::size_t m_cursor_position = 0;
        std::u32string m_input_text;

        /**
         * Positions the cursor widget at the given text position.
         * @param position Index in the range [0, text.length]
         */
        void move_cursor(std::size_t position);
    };
}
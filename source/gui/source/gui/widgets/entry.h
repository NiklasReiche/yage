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
		void update(double dt);
	};


	struct TextEntryTemplate : public WidgetTemplate 
	{
		gml::Vec2f padding = gml::Vec2f(2.0f);
		TextTemplate defaultText;
		TextTemplate inputText;
		unsigned int cursorColor = gl::Color::BLACK;
		int cursorWidth = 1;
		std::function<void()> command;
	};

	class TextEntry : public Widget
	{
	private:
		Label * label;
		TextCursor* cursor;
		CursorAnimation* cursorAnimation;

		gml::Vec2f padding = gml::Vec2f(2.0f);
		std::function<void()> callback;

		unsigned int cursorColor = gl::Color::BLACK;
		int cursorWidth = 1;
		int cursorPosition = 0;

		TextTemplate defaultTextTemplate;
		TextTemplate inputTextTemplate;
		std::string inputText = "";

		void moveCursor();

	public:
		TextEntry(Widget * parent, Master* master, TextEntryTemplate entryTemplate, Master* m);

		void onFocus();
		void onFocusRelease();
		void onCharInput(char character);
		void onKeyPress(input::KeyEvent::Code key);

		std::string getString();

		gml::Vec2f calcPrefSize();
	};
}
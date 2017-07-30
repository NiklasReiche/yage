#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "../core.h"
#include "../InputManager.h"
#include "../Widget.h"
#include "Label.h"

namespace gui
{
	struct TextEntryTemplate : public WidgetTemplate 
	{
		TextTemplate defaultText;
	};

	class TextEntry : public Widget
	{
	private:
		Label * label;
		bool isFocused = false;
		std::string defaultText = "";
		std::string text = "";

	public:
		TextEntry(Widget * parent, MasterInterface master, TextEntryTemplate entryTemplate);

		void onFocus();
		void onFocusRelease();
		void onCharInput(char character);
		void onKeyPress(input::KeyCode key);
	};
}
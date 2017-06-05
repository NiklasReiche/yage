#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "core.h"

#include "input.h"

#include "widget.h"
#include "label.h"


namespace gui
{
	class TextEntry : public Widget
	{
	private:
		Label * label;
		bool isFocused = false;
		std::string text;
	public:
		TextEntry(Widget * parent, ManagerInterface mInterface, W_Geometry geometry, W_Border border, unsigned int color);

		virtual void onClick();
		virtual void update();

		void setText(W_Text text) { label->setText(text); }
		void setTextColor(unsigned int color) { label->setTextColor(color); }
	};
}
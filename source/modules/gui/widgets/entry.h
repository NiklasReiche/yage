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
#if 0
	class TextEntry : public Widget
	{
	private:
		Label * label;
		bool isFocused = false;
		std::string text;

	public:
		TextEntry(Widget * parent, MasterInterface master, W_Geometry geometry, W_Border border, unsigned int color);

		virtual void onClick();
		virtual void update();

		void setText(TextLayout text) { label->setText(text); }
		void setTextColor(unsigned int color) { label->setTextColor(color); }
	};
#endif
}
#pragma once

#include <memory>

#include "../core.h"
#include "../widget.h"


namespace gui
{
	class Frame : public Widget
	{
	public:
		Frame(Widget * parent, Master* master, WidgetTemplate frameTemplate);
	};
}
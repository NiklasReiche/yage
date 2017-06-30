#pragma once

#include <memory>

#include "../core.h"
#include "../Widget.h"


namespace gui
{
	class Frame : public Widget
	{
	private:
		
	public:
		Frame(Widget * parent, MasterInterface master, WidgetLayout layout);
	};
}
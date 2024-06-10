#pragma once

#include <memory>

#include "widget.h"


namespace yage::gui
{
    /**
     * A simple widget that can be used as a frame around other widgets. Uses default layout rules and offers no
     * additional functionality.
     * TODO: Is this existence justified when we have container widgets?
     */
	class Frame : public Widget
	{
	public:
		Frame(Widget * parent, Master* master, const WidgetTemplate& widget_template);
	};
}
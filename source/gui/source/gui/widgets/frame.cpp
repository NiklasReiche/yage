#include "frame.h"

namespace yage::gui
{
	Frame::Frame(Widget * parent, Master* master, const WidgetTemplate& widget_template)
		: Widget(parent, master, widget_template)
	{
        m_is_interactable = false;
	}
}
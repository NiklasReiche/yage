#include "frame.h"

namespace gui
{
	Frame::Frame(Widget * parent, Master* master, WidgetTemplate frameTemplate)
		: Widget(parent, master, frameTemplate)
	{
        m_is_interactable = false;
	}
}
#include "Frame.h"

namespace gui
{
	Frame::Frame(Widget * parent, MasterInterface master, WidgetLayout layout)
		: Widget(parent, master, layout)
	{
		isInteractable = false;
	}
}
#pragma once

#include "widget.h"

namespace gui
{
	class RootWidget : public Widget
	{
	public:
		RootWidget();

		void relayout();
	};
}
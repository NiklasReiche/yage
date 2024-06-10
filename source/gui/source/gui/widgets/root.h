#pragma once

#include "widget.h"

namespace yage::gui
{
	class RootWidget : public Widget
	{
	public:
		explicit RootWidget(Master* master);
	};
}
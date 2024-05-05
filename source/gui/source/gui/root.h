#pragma once

#include "widget.h"

namespace gui
{
	class RootWidget : public Widget
	{
	public:
		explicit RootWidget(Master* master);

		void update_layout() override;
	};
}
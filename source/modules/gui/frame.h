#pragma once

#include <memory>

#include "core.h"
#include "widget.h"


namespace gui
{
	class Frame : public Widget
	{
	private:
		std::vector<std::shared_ptr<Widget>> children;
	public:
		Frame();
	};
}
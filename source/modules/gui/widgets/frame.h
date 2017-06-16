#pragma once

#include <memory>

#include "../core.h"
#include "../Widget.h"


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
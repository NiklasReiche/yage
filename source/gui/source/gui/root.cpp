#include "root.h"

namespace gui
{
	RootWidget::RootWidget()
		: Widget()
	{
		layout = std::make_unique<AbsoluteLayout>();
	}

	void RootWidget::relayout()
	{
		layout->update(this);
	}
}
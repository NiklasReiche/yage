#include "rootWidget.h"

namespace gui
{
	RootWidget::RootWidget()
		: Widget(nullptr, MasterInterface(), WidgetTemplate())
	{
		layout = std::make_unique<AbsoluteLayout>();
	}

	void RootWidget::relayout()
	{
		layout->update(this);
	}
}
#include "rootWidget.h"

namespace gui
{
	RootWidget::RootWidget()
		: Widget(nullptr, MasterInterface(), WidgetTemplate())
	{
		layout = std::make_unique<AbsoluteLayout>();
		parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_CHILDREN_FIXED);
	}

	void RootWidget::relayout()
	{
		layout->update(this);
	}
}
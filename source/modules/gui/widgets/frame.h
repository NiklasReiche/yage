#pragma once

#include <memory>

#include "../core.h"
#include "../Widget.h"


namespace gui
{
	struct FrameTemplate : public WidgetTemplate
	{
		LayoutType layoutType;
		gml::Vec2f layoutMargin;
		gml::Vec2<ParentSizeHint> parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_CHILDREN_RESIZE);
	};

	class Frame : public Widget
	{
	private:
		
	public:
		Frame(Widget * parent, MasterInterface master, FrameTemplate frameTemplate);
	};
}
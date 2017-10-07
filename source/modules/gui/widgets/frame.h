#pragma once

#include <memory>

#include "../core.h"
#include "../widget.h"


namespace gui
{
	struct FrameTemplate : public WidgetTemplate
	{
		LayoutType layoutType;
		gml::Vec2f layoutMargin;
	};

	class Frame : public Widget
	{
	private:
		
	public:
		Frame(Widget * parent, MasterInterface master, FrameTemplate frameTemplate);

		gml::Vec2f calcPrefSize();
	};
}
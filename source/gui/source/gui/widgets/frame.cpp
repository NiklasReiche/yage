#include "frame.h"

namespace gui
{
	Frame::Frame(Widget * parent, MasterInterface master, FrameTemplate frameTemplate)
		: Widget(parent, master, frameTemplate)
	{
		isInteractable = false;

		layoutMargin = frameTemplate.layoutMargin;
		
		if (prefSize == gml::Vec2f(0.0f)) {
			fitChildren = true;
		}

		switch (frameTemplate.layoutType) 
		{
		case LayoutType::ABSOLUTE_LAYOUT:
			layout = std::make_unique<AbsoluteLayout>();
			break;
		case LayoutType::V_LIST_LAYOUT:
			layout = std::make_unique<VListLayout>();
			break;
		case LayoutType::H_LIST_LAYOUT:
			layout = std::make_unique<HListLayout>();
			break;
		}
	}

	gml::Vec2f Frame::calcPrefSize()
	{
		gml::Vec2f mSize;
		mSize.x() = layout->calcPrefSizeX(this) + gml::Vec2f((float)borderSize).x();
		mSize.y() = layout->calcPrefSizeY(this) + gml::Vec2f((float)borderSize).y();
		return mSize;
	}
}
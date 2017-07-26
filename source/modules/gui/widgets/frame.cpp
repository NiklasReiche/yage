#include "Frame.h"

namespace gui
{
	Frame::Frame(Widget * parent, MasterInterface master, FrameTemplate frameTemplate)
		: Widget(parent, master, frameTemplate)
	{
		isInteractable = false;

		layoutMargin = frameTemplate.layoutMargin;
		parentSizeHint = frameTemplate.parentSizeHint;
		
		switch (parentSizeHint.x)
		{
		case ParentSizeHint::WRAP_CHILDREN_FIXED:
			childSizeHint.x = ChildSizeHint::MIN;
			break;
		case ParentSizeHint::WRAP_CHILDREN_RESIZE:
			childSizeHint.x = ChildSizeHint::MIN_EXPAND;
			break;
		case ParentSizeHint::WRAP_AROUND:
			childSizeHint.x = ChildSizeHint::FIXED;
			break;
		default:
			childSizeHint.x = ChildSizeHint::FIXED;
			break;
		}
		switch (parentSizeHint.y)
		{
		case ParentSizeHint::WRAP_CHILDREN_FIXED:
			childSizeHint.y = ChildSizeHint::MIN;
			break;
		case ParentSizeHint::WRAP_CHILDREN_RESIZE:
			childSizeHint.y = ChildSizeHint::MIN_EXPAND;
			break;
		case ParentSizeHint::WRAP_AROUND:
			childSizeHint.y = ChildSizeHint::FIXED;
			break;
		default:
			childSizeHint.y = ChildSizeHint::FIXED;
			break;
		}

		switch (frameTemplate.layoutType) 
		{
		case LayoutType::V_LIST_LAYOUT:
			layout = std::make_unique<VListLayout>();
			break;
		case LayoutType::H_LIST_LAYOUT:
			layout = std::make_unique<HListLayout>();
			break;
		}
	}
}
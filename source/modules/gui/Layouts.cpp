#include "Layouts.h"
#include "Widget.h"

namespace gui
{
	VListLayout::VListLayout()
		:Layout() {}

	void VListLayout::update(Widget* parent)
	{
		gml::Vec2<float> padding(5.0f, 5.0f);
		gml::Vec2<float> globalSize = parent->getSize();
		float slotSize = globalSize.y / parent->getChildrenCount();
		float offset = padding.y;
		
		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);
			child.setAnchor(Anchor::TOP_LEFT);
			child.move(gml::Vec2<float>(0.0f, offset));
			
			switch (parent->getParentSizeHint())
			{
			case ParentSizeHint::WRAP_CHILDREN_RESIZE:
				child.resize(gml::Vec2<float>(globalSize.x, slotSize));
				offset += slotSize;
				break;

			case ParentSizeHint::WRAP_CHILDREN_FIXED:
				offset += child.getSize().y;
				break;

			case ParentSizeHint::WRAP_AROUND:
				offset += child.getSize().y;
				parent->resize(gml::Vec2<float>(globalSize.x, offset + padding.y));
				break;
			}
		}	
	}
}
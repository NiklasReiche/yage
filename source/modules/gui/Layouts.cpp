#include "Layouts.h"
#include "Widget.h"

namespace gui
{
	VListLayout::VListLayout()
		:Layout() {}

	void VListLayout::update(Widget* parent)
	{
		gml::Vec2<float> globalSize = parent->getSize();
		float slotSize = globalSize.y / parent->getChildrenCount();
		float offset = 10.0f;

		
		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);
			child.setAnchor(Anchor::TOP_LEFT);
			child.move(gml::Vec2<float>(0.0f, offset));
			
			if (parent->getParentSizeHint() == ParentSizeHint::WRAP_CHILDREN) {
				child.resize(gml::Vec2<float>(globalSize.x, slotSize));
				offset += slotSize;
			}
			else if (parent->getParentSizeHint() == ParentSizeHint::WRAP_AROUND) {
				offset += child.getSize().y;
			}
		}
		
	}
}
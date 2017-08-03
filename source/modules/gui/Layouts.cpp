#include "Layouts.h"
#include "Widget.h"

namespace gui
{
	AbsoluteLayout::AbsoluteLayout()
		: Layout(LayoutType::ABSOLUTE_LAYOUT) {}
	gml::Vec2f AbsoluteLayout::calcParentPrefSize(Widget* parent)
	{
		return parent->toAbs(parent->getPreferredSize());
	}
	void AbsoluteLayout::update(Widget* widget)
	{
		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);
			child.setSize(child.toAbs(child.getPreferredSize()));
		}
	}


	VListLayout::VListLayout()
		:Layout(LayoutType::V_LIST_LAYOUT) {}
	gml::Vec2f VListLayout::calcParentPrefSize(Widget* parent)
	{
		gml::Vec2f parentPrefSize;

		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);

			gml::Vec2f childPrefSize = child.getPreferredSize() + child.getCellMargin();
			gml::Vec2<ChildSizeHint> sizeHint = child.getChildSizeHint();

			switch (sizeHint.x)
			{
			case ChildSizeHint::FIXED:
				parentPrefSize.x = std::max(childPrefSize.x, parentPrefSize.x);
				break;
			case ChildSizeHint::MIN:
				parentPrefSize.x = std::max(childPrefSize.x, parentPrefSize.x);
				break;
			}
			
			switch (sizeHint.y)
			{
			case ChildSizeHint::FIXED:
				parentPrefSize.y += childPrefSize.y;
				break;
			case ChildSizeHint::MIN:
				parentPrefSize.y += childPrefSize.y;
				break;
			}
		}
		return parentPrefSize + parent->getLayoutMargin() * 2;
	}
	void VListLayout::update(Widget* widget)
	{
		gml::Vec2f globalSize = widget->getInnerSize() - widget->getLayoutMargin() * 2;
		float slotSize = globalSize.y / widget->getChildrenCount();
		gml::Vec2f offset = widget->getLayoutMargin();
		
		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);
			child.setAnchor(Anchor::TOP_LEFT);
			
			gml::Vec2f childSizePref = child.getPreferredSize();
			gml::Vec2<ChildSizeHint> childSizeHint = child.getChildSizeHint();
			gml::Vec2f childSize;

			offset += child.getCellMargin();
			child.move(offset);

			switch (widget->getParentSizeHint().y)
			{
			case ParentSizeHint::WRAP_CHILDREN_RESIZE:
				childSize.y = slotSize;
				offset.y += slotSize;
				break;

			case ParentSizeHint::WRAP_CHILDREN_FIXED:
				childSize.y = childSizePref.y;
				offset.y += childSizePref.y;
				break;

			case ParentSizeHint::WRAP_AROUND:
				childSize.y = childSizePref.y;
				offset.y += childSizePref.y;
				break;
			}

			switch (widget->getParentSizeHint().x)
			{
			case ParentSizeHint::WRAP_CHILDREN_RESIZE:
				childSize.x = globalSize.x;
				break;

			case ParentSizeHint::WRAP_CHILDREN_FIXED:
				childSize.x = childSizePref.x;
				break;

			case ParentSizeHint::WRAP_AROUND:
				childSize.x = childSizePref.x;
				break;
			}
			child.setSize(childSize);
			offset.x = widget->getLayoutMargin().x;
		}	
	}


	HListLayout::HListLayout()
		: Layout(LayoutType::H_LIST_LAYOUT) {}
	gml::Vec2f HListLayout::calcParentPrefSize(Widget* parent)
	{
		gml::Vec2f parentPrefSize;

		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);
			gml::Vec2<ChildSizeHint> sizeHint = child.getChildSizeHint();
			gml::Vec2f childPrefSize = child.getPreferredSize() + child.getCellMargin();

			switch (sizeHint.y)
			{
			case ChildSizeHint::FIXED:
				parentPrefSize.y = std::max(childPrefSize.y, parentPrefSize.y);
				break;
			case ChildSizeHint::MIN:
				parentPrefSize.y = std::max(childPrefSize.y, parentPrefSize.y);
				break;
			case ChildSizeHint::MIN_EXPAND:
				parentPrefSize.y = std::max(childPrefSize.y, parentPrefSize.y);
				break;
			}

			switch (sizeHint.x)
			{
			case ChildSizeHint::FIXED:
				parentPrefSize.x += childPrefSize.x;
				break;
			case ChildSizeHint::MIN:
				parentPrefSize.x += childPrefSize.x;
				break;
			case ChildSizeHint::MIN_EXPAND:
				parentPrefSize.x += childPrefSize.x;
				break;
			}
		}
		return parentPrefSize + parent->getLayoutMargin() * 2;
	}
	void HListLayout::update(Widget* widget)
	{
		gml::Vec2f globalSize = widget->getInnerSize() - widget->getLayoutMargin() * 2;
		float slotSize = globalSize.y / widget->getChildrenCount();
		gml::Vec2f offset = widget->getLayoutMargin();

		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);
			child.setAnchor(Anchor::TOP_LEFT);

			gml::Vec2f childSizePref = child.getPreferredSize();
			gml::Vec2<ChildSizeHint> childSizeHint = child.getChildSizeHint();
			gml::Vec2f childSize;

			offset += child.getCellMargin();
			child.move(offset);

			switch (widget->getParentSizeHint().x)
			{
			case ParentSizeHint::WRAP_CHILDREN_RESIZE:
				childSize.x = slotSize;
				offset.x += slotSize;
				break;

			case ParentSizeHint::WRAP_CHILDREN_FIXED:
				childSize.x = childSizePref.x;
				offset.x += childSizePref.x;
				break;

			case ParentSizeHint::WRAP_AROUND:
				childSize.x = childSizePref.x;
				offset.x += childSizePref.x;
				break;
			}

			switch (widget->getParentSizeHint().y)
			{
			case ParentSizeHint::WRAP_CHILDREN_RESIZE:
				childSize.y = globalSize.y;
				break;

			case ParentSizeHint::WRAP_CHILDREN_FIXED:
				childSize.y = childSizePref.y;
				break;

			case ParentSizeHint::WRAP_AROUND:
				childSize.y = childSizePref.y;
				break;
			}
			child.setSize(childSize);
			offset.y = widget->getLayoutMargin().y;
		}
	}


	GridLayout::GridLayout()
		: Layout(LayoutType::GRID_LAYOUT) {}
}
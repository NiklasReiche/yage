#include "layouts.h"
#include "widget.h"

namespace gui
{
	AbsoluteLayout::AbsoluteLayout()
		: Layout(LayoutType::ABSOLUTE_LAYOUT) {}
	void AbsoluteLayout::update(Widget* widget)
	{
		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);
			gml::Vec2f childPrefSize = child.getPreferredSize();
			gml::Vec2f childCellMargin = child.getCellMargin();

			if (child.getSizeHint().x == SizeHint::INFINITE) {
				childPrefSize.x = child.toAbsX(childPrefSize.x);
			}
			if (child.getSizeHint().y == SizeHint::INFINITE) {
				childPrefSize.y = child.toAbsY(childPrefSize.y);
			}
			if (child.getOffsetHint().x == OffsetHint::INFINITE) {
				childCellMargin.x = child.toAbsX(childCellMargin.x);
			}
			if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childCellMargin.y = child.toAbsY(childCellMargin.y);
			}

			child.setSize(childPrefSize);
			child.setOffset(childCellMargin);
		}
	}


	VListLayout::VListLayout()
		:Layout(LayoutType::V_LIST_LAYOUT) {}
	float VListLayout::calcPrefSizeX(Widget* parent)
	{
		float parentPrefSize = 0.0f;

		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);

			SizeHint childSizeHint = child.getSizeHint().x;
			float childPrefSize = 0.0f;
			float childMinSize = 0.0f;

			if (child.getOffsetHint().x == OffsetHint::FIXED) {
				childPrefSize = child.getPreferredSize().x + child.getCellMargin().x;
				childMinSize = child.getMinSize().x + child.getCellMargin().x;
			}
			else if (child.getOffsetHint().x == OffsetHint::INFINITE) {
				childPrefSize = child.getPreferredSize().x;
				childMinSize = child.getMinSize().x;
			}
			
			switch (childSizeHint)
			{
			case SizeHint::FIXED:
				parentPrefSize = std::max(parentPrefSize, childPrefSize);
				break;
			case SizeHint::ASPECT:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			case SizeHint::INFINITE:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			case SizeHint::EXPANDING:
				parentPrefSize = std::max(parentPrefSize, childPrefSize);
				break;
			case SizeHint::SHRINKING:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			case SizeHint::RECOMMENDED:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			}
		}

		return parentPrefSize + parent->getLayoutMargin().x * 2;
	}
	float VListLayout::calcPrefSizeY(Widget* parent)
	{
		float parentPrefSize = 0.0f;

		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);

			SizeHint childSizeHint = child.getSizeHint().y;
			float childPrefSize = 0.0f;
			float childMinSize = 0.0f;

			if (child.getOffsetHint().y == OffsetHint::FIXED) {
				childPrefSize = child.getPreferredSize().y + child.getCellMargin().y;
				childMinSize = child.getMinSize().y + child.getCellMargin().y;
			}
			else if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childPrefSize = child.getPreferredSize().y;
				childMinSize = child.getMinSize().y;
			}

			switch (childSizeHint)
			{
			case SizeHint::FIXED:
				parentPrefSize += childPrefSize;
				break;
			case SizeHint::ASPECT:
				parentPrefSize += childMinSize;
				break;
			case SizeHint::INFINITE:
				parentPrefSize += childMinSize;
				break;
			case SizeHint::EXPANDING:
				parentPrefSize += childPrefSize;
				break;
			case SizeHint::SHRINKING:
				parentPrefSize += childMinSize;
				break;
			case SizeHint::RECOMMENDED:
				parentPrefSize += childMinSize;
				break;
			}
		}

		return parentPrefSize + parent->getLayoutMargin().y * 2;
	}

	void VListLayout::update(Widget* widget)
	{
		gml::Vec2f totalSize = widget->getInnerSize() - widget->getLayoutMargin() * 2;
		gml::Vec2f offset = widget->getLayoutMargin();

		float totalShrinkingSize = 0.0f;
		float shrinkPercent = 0.0f;
		int nExpanding = 0;
		float expandSize = 0.0f;

		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);

			gml::Vec2f childCellMargin = child.getCellMargin();
			if (child.getOffsetHint().x == OffsetHint::INFINITE) {
				childCellMargin.x = child.toAbsX(childCellMargin.x);
			}
			if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childCellMargin.y = child.toAbsY(childCellMargin.y);
			}

			if (child.getSizeHint().y == SizeHint::INFINITE) {
				totalSize.y -= child.toAbsY(child.getPreferredSize().y) + childCellMargin.y;
			}
			else if (child.getSizeHint().y == SizeHint::ASPECT) {
				totalSize.y -= child.fromAspect() + childCellMargin.y;
			}
			else {
				totalSize.y -= child.getPreferredSize().y + childCellMargin.y;

				if (child.getSizeHint().y == SizeHint::SHRINKING || child.getSizeHint().y == SizeHint::RECOMMENDED) {
					totalShrinkingSize += child.getPreferredSize().y - child.getMinSize().y;
				}
				if (child.getSizeHint().y == SizeHint::EXPANDING || child.getSizeHint().y == SizeHint::RECOMMENDED) {
					++nExpanding;
				}
			}
		}

		if (totalSize.y < 0) {
			shrinkPercent = (-1 * totalSize.y) / totalShrinkingSize;
		}
		else if (totalSize.y > 0) {
			expandSize = totalSize.y / nExpanding;
		}
		
		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);
			child.setAnchor(Anchor::TOP_LEFT);
			
			gml::Vec2<SizeHint> childSizeHint = child.getSizeHint();
			gml::Vec2f childPrefSize = child.getPreferredSize();
			gml::Vec2f childMinSize = child.getMinSize();
			gml::Vec2f childCellMargin = child.getCellMargin();
			gml::Vec2f childSize(0.0f);

			if (child.getOffsetHint().x == OffsetHint::INFINITE) {
				childCellMargin.x = child.toAbsX(childCellMargin.x);
			}
			if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childCellMargin.y = child.toAbsY(childCellMargin.y);
			}

			offset += childCellMargin;
			child.setOffset(offset);

			switch (childSizeHint.x)
			{
			case SizeHint::FIXED:
				childSize.x = childPrefSize.x;
				break;
			case SizeHint::INFINITE:
				childSize.x = child.toAbsX(childPrefSize.x);
				break;
			case SizeHint::ASPECT:
				childSize.x = child.fromAspect();
				break;
			case SizeHint::EXPANDING:
				childSize.x = totalSize.x - childCellMargin.x;
				break;
			case SizeHint::SHRINKING:
				childSize.x = totalSize.x - childCellMargin.x;
				break;
			case SizeHint::RECOMMENDED:
				childSize.x = totalSize.x - childCellMargin.x;
				break;
			}

			switch (childSizeHint.y)
			{
			case SizeHint::FIXED:
				childSize.y = childPrefSize.y;
				break;
			case SizeHint::INFINITE:
				childSize.y = child.toAbsY(childPrefSize.y);
				break;
			case SizeHint::ASPECT:
				childSize.y = child.fromAspect();
				break;
			case SizeHint::EXPANDING:
				childSize.y = childPrefSize.y + expandSize;
				break;
			case SizeHint::SHRINKING:
				childSize.y = childPrefSize.y - ((childPrefSize.y - childMinSize.y) * shrinkPercent);
				break;
			case SizeHint::RECOMMENDED:
				if (shrinkPercent > 0) {
					childSize.y = childPrefSize.y - ((childPrefSize.y - childMinSize.y) * shrinkPercent);
				}
				else if (expandSize > 0) {
					childSize.y = childPrefSize.y + expandSize;
				}
				else {
					childSize.y = childPrefSize.y;
				}
				break;
			}
			
			child.setSize(childSize);
			offset.x = widget->getLayoutMargin().x;
			offset.y += childSize.y;
		}	
	}


	HListLayout::HListLayout()
		: Layout(LayoutType::H_LIST_LAYOUT) {}
	float HListLayout::calcPrefSizeX(Widget* parent)
	{
		float parentPrefSize = 0.0f;

		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);

			SizeHint childSizeHint = child.getSizeHint().x;
			float childPrefSize = 0.0f;
			float childMinSize = 0.0f;

			if (child.getOffsetHint().x == OffsetHint::FIXED) {
				childPrefSize = child.getPreferredSize().x + child.getCellMargin().x;
				childMinSize = child.getMinSize().x + child.getCellMargin().x;
			}
			else if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childPrefSize = child.getPreferredSize().x;
				childMinSize = child.getMinSize().x;
			}

			switch (childSizeHint)
			{
			case SizeHint::FIXED:
				parentPrefSize += childPrefSize;
				break;
			case SizeHint::ASPECT:
				parentPrefSize += childMinSize;
				break;
			case SizeHint::INFINITE:
				parentPrefSize += childMinSize;
				break;
			case SizeHint::EXPANDING:
				parentPrefSize += childPrefSize;
				break;
			case SizeHint::SHRINKING:
				parentPrefSize += childMinSize;
				break;
			case SizeHint::RECOMMENDED:
				parentPrefSize += childMinSize;
				break;
			}
		}

		return parentPrefSize + parent->getLayoutMargin().x * 2;
	}
	float HListLayout::calcPrefSizeY(Widget* parent)
	{
		float parentPrefSize = 0.0f;

		for (unsigned int i = 0; i < parent->getChildrenCount(); ++i)
		{
			Widget & child = parent->getChild(i);

			SizeHint childSizeHint = child.getSizeHint().y;
			float childPrefSize = 0.0f;
			float childMinSize = 0.0f;

			if (child.getOffsetHint().y == OffsetHint::FIXED) {
				childPrefSize = child.getPreferredSize().y + child.getCellMargin().y;
				childMinSize = child.getMinSize().y + child.getCellMargin().y;
			}
			else if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childPrefSize = child.getPreferredSize().y;
				childMinSize = child.getMinSize().y;
			}

			switch (childSizeHint)
			{
			case SizeHint::FIXED:
				parentPrefSize = std::max(parentPrefSize, childPrefSize);
				break;
			case SizeHint::ASPECT:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			case SizeHint::INFINITE:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			case SizeHint::EXPANDING:
				parentPrefSize = std::max(parentPrefSize, childPrefSize);
				break;
			case SizeHint::SHRINKING:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			case SizeHint::RECOMMENDED:
				parentPrefSize = std::max(parentPrefSize, childMinSize);
				break;
			}
		}

		return parentPrefSize + parent->getLayoutMargin().y * 2;
	}

	void HListLayout::update(Widget* widget)
	{
		gml::Vec2f totalSize = widget->getInnerSize() - widget->getLayoutMargin() * 2;
		gml::Vec2f offset = widget->getLayoutMargin();

		float totalShrinkingSize = 0.0f;
		float shrinkPercent = 0.0f;
		int nExpanding = 0;
		float expandSize = 0.0f;

		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);

			gml::Vec2f childCellMargin = child.getCellMargin();
			if (child.getOffsetHint().x == OffsetHint::INFINITE) {
				childCellMargin.x = child.toAbsX(childCellMargin.x);
			}
			if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childCellMargin.y = child.toAbsY(childCellMargin.y);
			}

			if (child.getSizeHint().x == SizeHint::INFINITE) {
				totalSize.x -= child.toAbsX(child.getPreferredSize().x) + childCellMargin.x;
			}
			else if (child.getSizeHint().y == SizeHint::ASPECT) {
				totalSize.y -= child.fromAspect() + childCellMargin.x;
			}
			else {
				totalSize.x -= child.getPreferredSize().x + childCellMargin.x;

				if (child.getSizeHint().x == SizeHint::SHRINKING || child.getSizeHint().x == SizeHint::RECOMMENDED) {
					totalShrinkingSize += child.getPreferredSize().x - child.getMinSize().x;
				}
				if (child.getSizeHint().x == SizeHint::EXPANDING || child.getSizeHint().x == SizeHint::RECOMMENDED) {
					++nExpanding;
				}
			}
		}

		if (totalSize.x < 0) {
			shrinkPercent = (-1 * totalSize.x) / totalShrinkingSize;
		}
		else if (totalSize.x > 0) {
			expandSize = totalSize.x / nExpanding;
		}

		for (unsigned int i = 0; i < widget->getChildrenCount(); ++i)
		{
			Widget & child = widget->getChild(i);
			child.setAnchor(Anchor::TOP_LEFT);

			gml::Vec2<SizeHint> childSizeHint = child.getSizeHint();
			gml::Vec2f childPrefSize = child.getPreferredSize();
			gml::Vec2f childMinSize = child.getMinSize();
			gml::Vec2f childCellMargin = child.getCellMargin();
			gml::Vec2f childSize(0.0f);

			if (child.getOffsetHint().x == OffsetHint::INFINITE) {
				childCellMargin.x = child.toAbsX(childCellMargin.x);
			}
			if (child.getOffsetHint().y == OffsetHint::INFINITE) {
				childCellMargin.y = child.toAbsY(childCellMargin.y);
			}

			offset += childCellMargin;
			child.setOffset(offset);

			switch (childSizeHint.y)
			{
			case SizeHint::FIXED:
				childSize.y = childPrefSize.y;
				break;
			case SizeHint::INFINITE:
				childSize.y = child.toAbsY(childPrefSize.y);
				break;
			case SizeHint::ASPECT:
				childSize.y = child.fromAspect();
				break;
			case SizeHint::EXPANDING:
				childSize.y = totalSize.y - childCellMargin.y;
				break;
			case SizeHint::SHRINKING:
				childSize.y = totalSize.y - childCellMargin.y;
				break;
			case SizeHint::RECOMMENDED:
				childSize.y = totalSize.y - childCellMargin.y;
				break;
			}

			switch (childSizeHint.x)
			{
			case SizeHint::FIXED:
				childSize.x = childPrefSize.x;
				break;
			case SizeHint::INFINITE:
				childSize.x = child.toAbsX(childPrefSize.x);
				break;
			case SizeHint::ASPECT:
				childSize.x = child.fromAspect();
				break;
			case SizeHint::EXPANDING:
				childSize.x = childPrefSize.x + expandSize;
				break;
			case SizeHint::SHRINKING:
				childSize.x = childPrefSize.x - ((childPrefSize.x - childMinSize.x) * shrinkPercent);
				break;
			case SizeHint::RECOMMENDED:
				if (shrinkPercent > 0) {
					childSize.x = childPrefSize.x - ((childPrefSize.x - childMinSize.x) * shrinkPercent);
				}
				else if (expandSize > 0) {
					childSize.x = childPrefSize.x + expandSize;
				}
				else {
					childSize.x = childPrefSize.x;
				}
				break;
			}

			child.setSize(childSize);
			offset.y = widget->getLayoutMargin().y;
			offset.x += childSize.x;
		}
	}
	

	GridLayout::GridLayout()
		: Layout(LayoutType::GRID_LAYOUT) {}
}
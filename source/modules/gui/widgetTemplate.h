#pragma once

#include "math/MVector.h"
#include "graphics/Color.h"

#include "Layouts.h"

namespace gui
{
	enum class Anchor
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		CENTER
	};
	enum class ValueType
	{
		ABSOLUTE,
		RELATIVE
	};
	

	struct WidgetLayoutTemplate
	{
		LayoutType layout;
		gml::Vec2<ParentSizeHint> parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_CHILDREN_RESIZE);
		gml::Vec2<ChildSizeHint> childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
	};
	struct WidgetGeometryTemplate
	{
		Anchor anchor = Anchor::TOP_LEFT;
		ValueType offsetType = ValueType::ABSOLUTE;
		gml::Vec2f offset;
		ValueType sizeType = ValueType::ABSOLUTE;
		gml::Vec2f size;
	};
	struct WidgetBorderTemplate
	{
		int size = 0;
		unsigned int color = gl::Color::BLACK;
	};
	struct WidgetShadowTemplate
	{
		int offset = 0;
		float hardness = 0.5f;
	};

	struct WidgetTemplate
	{
		WidgetGeometryTemplate geometry;
		WidgetBorderTemplate border;
		WidgetShadowTemplate shadow;
		unsigned int color = gl::Color::WHITE;
	};
}
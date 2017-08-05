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

	struct WidgetGeometryTemplate
	{
		Anchor anchor = Anchor::TOP_LEFT;
		gml::Vec2f offset;
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
		gml::Vec2<SizeHint> sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
		gml::Vec2<OffsetHint> offsetHint = gml::Vec2<OffsetHint>(OffsetHint::FIXED);
	};
}
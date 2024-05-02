#pragma once

#include "gml/vector.h"
#include "core/gl/color.h"
#include <core/gl/graphics.h>

#include "layouts.h"

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
		gml::Vec2f minSize = gml::Vec2f(0.0f);
		gml::Vec2f maxSize = gml::Vec2f(0.0f);
		gml::Vec2<SizeHint> sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
		gml::Vec2<OffsetHint> offsetHint = gml::Vec2<OffsetHint>(OffsetHint::FIXED);
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
	struct WidgetTextureTemplate
	{
		std::string filename;
		float resizeFactor = 1.0f;
	};

	struct WidgetTemplate
	{
		WidgetGeometryTemplate geometry;
		WidgetBorderTemplate border;
		WidgetShadowTemplate shadow;
		unsigned int color = gl::Color::WHITE;
		WidgetTextureTemplate texture;
	};
}
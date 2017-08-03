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
	
	class GeoVec
	{
	public:
		gml::Vec2f value;
		ValueType type;

		GeoVec()
			: value(gml::Vec2f(0.0f)), type(ValueType::ABSOLUTE) {}
		GeoVec(gml::Vec2f value, ValueType type = ValueType::ABSOLUTE)
			: value(value), type(type) {}
		GeoVec(float x, float y, ValueType type = ValueType::ABSOLUTE)
			: value(gml::Vec2f(x, y)), type(type) {}

		friend bool operator==(GeoVec & left, GeoVec & right);
		friend bool operator!=(GeoVec & left, GeoVec & right);
	};
	inline bool operator==(GeoVec & left, GeoVec & right)
	{
		return (left.value == right.value && left.type == right.type);
	}
	inline bool operator!=(GeoVec & left, GeoVec & right)
	{
		return !(left == right);
	}


	struct WidgetLayoutTemplate
	{
		LayoutType layout;
		gml::Vec2<ParentSizeHint> parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_CHILDREN_RESIZE);
		gml::Vec2<ChildSizeHint> childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
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
		gml::Vec2<ChildSizeHint> childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
	};
}
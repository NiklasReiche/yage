#pragma once

#include "core.h"

namespace gui
{
	class Widget;

	enum class ParentSizeHint
	{
		WRAP_CHILDREN_RESIZE,
		WRAP_CHILDREN_FIXED,
		WRAP_AROUND
	};
	enum class ChildSizeHint
	{
		MIN,
		MIN_EXPAND,
		FIXED,
		ASPECT
	};
	enum class LayoutType
	{
		ABSOLUTE_LAYOUT,
		V_LIST_LAYOUT,
		H_LIST_LAYOUT,
		GRID_LAYOUT
	};

	class Layout
	{
	protected:
		LayoutType type;

	public:
		Layout(){}
		Layout(LayoutType type)
			: type(type) {}
		virtual ~Layout() {}
		virtual gml::Vec2f calcParentPrefSize(Widget* parent) { return gml::Vec2f(); }
		virtual void update(Widget* widget) {}
	};


	class AbsoluteLayout : public Layout
	{
	public:
		AbsoluteLayout();
		void update(Widget* parent);
	};


	class VListLayout : public Layout
	{
	public:
		VListLayout();
		gml::Vec2f calcParentPrefSize(Widget* parent);
		void update(Widget* parent);
	};


	class HListLayout : public Layout
	{
	public:
		HListLayout();
		gml::Vec2f calcParentPrefSize(Widget* parent);
		void update(Widget* parent);
	};


	class GridLayout : public Layout
	{
	public:
		GridLayout();
		//void update(Widget* parent);
	};
}
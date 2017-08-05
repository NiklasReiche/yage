#pragma once

#include "core.h"

namespace gui
{
	class Widget;

	enum class SizeHint
	{
		FIXED,
		EXPANDING,
		SHRINKING,
		RECOMMENDED,
		INFINITE
	};
	enum class OffsetHint
	{
		FIXED,
		INFINITE
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
		virtual float calcPrefSizeX(Widget* parent) { return 0.0f; }
		virtual float calcPrefSizeY(Widget* parent) { return 0.0f; }
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
		float calcPrefSizeX(Widget* parent);
		float calcPrefSizeY(Widget* parent);
		void update(Widget* parent);
	};


	class HListLayout : public Layout
	{
	public:
		HListLayout();
		float calcPrefSizeX(Widget* parent);
		float calcPrefSizeY(Widget* parent);
		void update(Widget* parent);
	};


	class GridLayout : public Layout
	{
	public:
		GridLayout();
	};
}
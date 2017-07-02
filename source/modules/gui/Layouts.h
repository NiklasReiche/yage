#pragma once

#include "core.h"

namespace gui
{
	class Widget;

	enum class LayoutType
	{
		V_LIST_LAYOUT,
		H_LIST_LAYOUT,
		GRID_LAYOUT
	};

	class Layout
	{
	public:
		Layout() {}
		virtual ~Layout() {}
		virtual void update(Widget* parent) {}
	};

	class AbsoluteLayout : public Layout
	{
	public:
		void update(Widget* parent);
	};

	class VListLayout : public Layout
	{
	public:
		VListLayout();
		void update(Widget* parent);
	};

	class HListLayout : public Layout
	{
	public:
		void update(Widget* parent);
	};

	class GridLayout : public Layout
	{
	public:
		void update(Widget* parent);
	};
}
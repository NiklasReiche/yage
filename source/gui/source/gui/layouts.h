#pragma once

#include "core.h"

namespace gui
{
	class Widget;

	enum class SizeHint
	{
        /** This element should stay at a fixed size */
		FIXED,
        /** This element should expand with the parent container */
		EXPANDING,
        /** This element should shrink with the parent container */
		SHRINKING,
        /**  */
		RECOMMENDED,
        /**  */
		INFINITE, // TODO: this should probably be called RELATIVE, and also not be part of SizeHint
        /**  */
		ASPECT
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
	};

	class Layout
	{
	protected:
		LayoutType type;

	public:
        Layout() = default;

        explicit Layout(LayoutType type) : type(type){}

		virtual ~Layout() = default;

		virtual float calcPrefSizeX(Widget* parent) { return 0.0f; }

		virtual float calcPrefSizeY(Widget* parent) { return 0.0f; }

		virtual void update(Widget* widget) {}
	};

	class AbsoluteLayout : public Layout
	{
	public:
        AbsoluteLayout() : Layout(LayoutType::ABSOLUTE_LAYOUT){}

		void update(Widget* parent) override;
	};

	class VListLayout : public Layout
	{
	public:
        VListLayout() : Layout(LayoutType::V_LIST_LAYOUT)
        {}

		float calcPrefSizeX(Widget* parent) override;

        float calcPrefSizeY(Widget* parent) override;

		void update(Widget* parent) override;
	};

	class HListLayout : public Layout
	{
	public:
        HListLayout() : Layout(LayoutType::H_LIST_LAYOUT)
        {}

		float calcPrefSizeX(Widget* parent) override;

		float calcPrefSizeY(Widget* parent) override;

		void update(Widget* parent) override;
	};
}
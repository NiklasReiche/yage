#pragma once

#include "widget.h"

namespace gui
{
    struct ListBoxTemplate
    {
        enum Orientation
        {
            /**
             * Stacks children along the x-axis.
             */
            HORIZONTAL = 0,
            /**
             * Stacks children along the y-axis.
             */
            VERTICAL = 1
        };

        WidgetTemplate base {};
        /**
         * Orientation of the children in the list.
         */
        Orientation orientation = VERTICAL;
        /**
         * Spacing between children in the list. Does not get applied to the start end end of list, use padding instead.
         */
        float spacing = 0.0f;
    };

    /**
     * Container widget that stacks its children horizontally or vertically. Ignores anchors and offsets on children,
     * but respects sizing constraints.
     */
    class ListBox : public Widget
    {
    public:
        ListBox(Widget* parent, Master* master, const ListBoxTemplate& list_box_template);

        void update_layout() override;

        [[nodiscard]]
        gml::Vec2f preferred_size() const override;

    private:
        ListBoxTemplate m_box_template;

        [[nodiscard]]
        float pref_size_primary_axis() const;

        [[nodiscard]]
        float pref_size_secondary_axis() const;

        [[nodiscard]]
        float required_spacing() const;

        [[nodiscard]]
        int primary_axis() const;

        [[nodiscard]]
        int secondary_axis() const;
    };
}

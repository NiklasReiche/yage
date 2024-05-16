#pragma once

#include "widget.h"

namespace gui
{
    /**
     * Container widget that stacks its children horizontally. Ignores anchors and offsets on children, but respects
     * sizing constraints.
     * TODO: merge with VListBox
     * TODO: add spacing option
     */
    class HListBox : public Widget
    {
    public:
        HListBox(Widget* parent, Master* master, const WidgetTemplate& widget_template);

        void update_layout() override;

        [[nodiscard]]
        gml::Vec2f preferred_size() const override;

    private:
        [[nodiscard]]
        float calc_pref_size_x() const;

        [[nodiscard]]
        float calc_pref_size_y() const;
    };
}

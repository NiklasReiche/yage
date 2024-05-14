#pragma once

#include "gui/widget.h"

namespace gui
{
    /**
     * Container widget that stacks its children vertically. Ignores anchors and offsets on children, but respects
     * sizing constraints.
     * TODO: merge with HListBox
     */
    class VListBox : public Widget
    {
    public:
        VListBox(Widget* parent, Master* master, const WidgetTemplate& widget_template);

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

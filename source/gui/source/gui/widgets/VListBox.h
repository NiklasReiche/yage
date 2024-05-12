#pragma once

#include "gui/widget.h"

namespace gui
{

    class VListBox : public Widget
    {
    public:
        VListBox(Widget* parent, Master* master, const WidgetTemplate& widget_template);

        void update_layout() override;

        gml::Vec2f preferred_size() override;

    private:
        float calc_pref_size_x();

        float calc_pref_size_y();
    };

}

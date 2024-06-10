#include "listBox.h"

namespace yage::gui
{
    ListBox::ListBox(Widget* parent, Master* master, const ListBoxTemplate& widget_template)
            : Widget(parent, master, widget_template.base),
              m_box_template(widget_template)
    {
    }

    void ListBox::update_layout()
    {
        auto axis_first = primary_axis();
        auto axis_second = secondary_axis();

        float remaining_size = m_inner_size_abs(axis_first);
        float potential_shrink_size = 0.0f;
        int n_expanding = 0;

        remaining_size -= required_spacing();

        for (auto& child: m_children) {
            // try to fit children with preferred size
            remaining_size -= child->preferred_size()(axis_first);

            // count how much we can potentially shrink
            if (child->size_hint()(axis_first) == SizeHint::SHRINKING ||
                child->size_hint()(axis_first) == SizeHint::EXPANDING_SHRINKING) {
                potential_shrink_size += child->preferred_size()(axis_first) - child->min_size()(axis_first);
            }

            // track how many children want to expand
            if (child->size_hint()(axis_first) == SizeHint::EXPANDING ||
                child->size_hint()(axis_first) == SizeHint::EXPANDING_SHRINKING) {
                ++n_expanding;
            }
        }

        float shrink_percent = 0.0f;
        float expand_size_per_child = 0.0f;
        if (remaining_size < 0) {
            shrink_percent = -1 * remaining_size / potential_shrink_size; // TODO: division by zero / negative numbers
        } else if (remaining_size > 0) {
            // each expanding child gets equal amounts of expansion space
            expand_size_per_child = remaining_size / static_cast<float>(n_expanding);
        }

        float offset_primary_axis = 0;
        for (auto& child: m_children) {
            math::Vec2f child_pref_size = child->preferred_size();
            math::Vec2f child_min_size = child->min_size();
            math::Vec2f child_size(0.0f);

            child->override_anchor({.position = AnchorPosition::TOP_LEFT});
            math::Vec2f child_offset(0);
            child_offset(axis_first) = offset_primary_axis;
            child->set_absolute_offset(child_offset);

            switch (child->size_hint()(axis_second)) {
                case SizeHint::FIXED:
                case SizeHint::FIT_CHILDREN:
                    child_size(axis_second) = child_pref_size(axis_second);
                    break;
                case SizeHint::EXPANDING: // TODO: these should be min/max
                case SizeHint::SHRINKING: // TODO: these should be min/max
                case SizeHint::EXPANDING_SHRINKING: // TODO: these should be min/max
                    child_size(axis_second) = m_inner_size_abs(axis_second);
                    break;
            }

            switch (child->size_hint()(axis_first)) {
                case SizeHint::FIXED:
                case SizeHint::FIT_CHILDREN:
                    child_size(axis_first) = child_pref_size(axis_first);
                    break;
                case SizeHint::EXPANDING:
                    child_size(axis_first) = child_pref_size(axis_first) + expand_size_per_child;
                    break;
                case SizeHint::SHRINKING:
                    child_size(axis_first) =
                            child_pref_size(axis_first) - ((child_pref_size(axis_first) - child_min_size(
                                    axis_first)) * shrink_percent);
                    break;
                case SizeHint::EXPANDING_SHRINKING:
                    if (shrink_percent > 0) {
                        child_size(axis_first) =
                                child_pref_size(axis_first) - ((child_pref_size(axis_first) - child_min_size(
                                        axis_first)) * shrink_percent);
                    } else if (expand_size_per_child > 0) {
                        child_size(axis_first) = child_pref_size(axis_first) + expand_size_per_child;
                    } else {
                        child_size(axis_first) = child_pref_size(axis_first);
                    }
                    break;
            }

            offset_primary_axis += child_size(axis_first) + m_box_template.spacing;

            child->set_actual_size(child_size);
            child->update_layout();
        }
    }

    math::Vec2f ListBox::preferred_size() const
    {
        math::Vec2f inner_size(0);
        inner_size(primary_axis()) = pref_size_primary_axis();
        inner_size(secondary_axis()) = pref_size_secondary_axis();

        math::Vec2f spacing(0);
        spacing(primary_axis()) = required_spacing();

        return inner_size +
               math::Vec2f(m_template.border.thickness * 2) +
               math::Vec2f(m_template.padding(0), m_template.padding(1)) +
               math::Vec2f(m_template.padding(2), m_template.padding(3)) +
               spacing;
    }

    float ListBox::pref_size_primary_axis() const
    {
        auto axis = primary_axis();

        float preferred_size = 0.0f;
        for (auto& child: m_children) {
            switch (child->size_hint()(axis)) {
                case SizeHint::FIXED:
                case SizeHint::EXPANDING:
                case SizeHint::FIT_CHILDREN:
                    preferred_size += child->preferred_size()(axis);
                    break;
                case SizeHint::SHRINKING:
                case SizeHint::EXPANDING_SHRINKING:
                    preferred_size = child->min_size()(axis);
                    break;
            }
        }

        return preferred_size;
    }

    float ListBox::pref_size_secondary_axis() const
    {
        auto axis = secondary_axis();

        float preferred_size = 0.0f;
        for (auto& child: m_children) {
            switch (child->size_hint()(axis)) {
                case SizeHint::FIXED:
                case SizeHint::EXPANDING:
                case SizeHint::FIT_CHILDREN:
                    preferred_size = std::max(preferred_size, child->preferred_size()(axis));
                    break;
                case SizeHint::SHRINKING:
                case SizeHint::EXPANDING_SHRINKING:
                    preferred_size = std::max(preferred_size, child->min_size()(axis));
                    break;
            }
        }

        return preferred_size;
    }

    float ListBox::required_spacing() const
    {
        if (m_children.size() > 1) {
            return static_cast<float>(m_children.size() - 1) * m_box_template.spacing;
        }
        return 0.0f;
    }

    int ListBox::primary_axis() const
    {
        return m_box_template.orientation;
    }

    int ListBox::secondary_axis() const
    {
        return m_box_template.orientation == ListBoxTemplate::VERTICAL ? ListBoxTemplate::HORIZONTAL
                                                                       : ListBoxTemplate::VERTICAL;
    }
}

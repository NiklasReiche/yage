#include "HListBox.h"

namespace gui
{
    void HListBox::update_layout()
    {
        float remaining_size = m_inner_size_abs.x();
        float potentialShrinkSize = 0.0f;
        int nExpanding = 0;

        for (auto& child : m_children) {
            // try to fit children with preferred size
            remaining_size -= child->preferred_size().x();

            // count how much we can potentially shrink
            if (child->size_hint().x() == SizeHint::SHRINKING || child->size_hint().x() == SizeHint::EXPANDING_SHRINKING) {
                potentialShrinkSize += child->preferred_size().x() - child->min_size().x();
            }

            // track how many children want to expand
            if (child->size_hint().x() == SizeHint::EXPANDING || child->size_hint().x() == SizeHint::EXPANDING_SHRINKING) {
                ++nExpanding;
            }
        }

        float shrinkPercent = 0.0f;
        float expandSizePerChild = 0.0f;
        if (remaining_size < 0) {
            shrinkPercent = -1 * remaining_size / potentialShrinkSize; // TODO: division by zero / negative numbers
        } else if (remaining_size > 0) {
            // each expanding child gets equal amounts of expansion space
            expandSizePerChild = remaining_size / nExpanding;
        }

        float x_offset = 0;
        for (auto& child: m_children) {
            gml::Vec2f childPrefSize = child->preferred_size();
            gml::Vec2f childMinSize = child->min_size();
            gml::Vec2f childSize(0.0f);

            child->set_actual_anchor({.position = AnchorPosition::TOP_LEFT});
            child->set_absolute_offset({x_offset, 0});

            switch (child->size_hint().y()) {
                case SizeHint::FIXED:
                case SizeHint::FIT_CHILDREN:
                    childSize.y() = childPrefSize.y();
                    break;
                case SizeHint::EXPANDING: // TODO: these should be min/max
                case SizeHint::SHRINKING: // TODO: these should be min/max
                case SizeHint::EXPANDING_SHRINKING: // TODO: these should be min/max
                    childSize.y() = m_inner_size_abs.y();
                    break;
            }

            switch (child->size_hint().x()) {
                case SizeHint::FIXED:
                case SizeHint::FIT_CHILDREN:
                    childSize.x() = childPrefSize.x();
                    break;
                case SizeHint::EXPANDING:
                    childSize.x() = childPrefSize.x() + expandSizePerChild;
                    break;
                case SizeHint::SHRINKING:
                    childSize.x() = childPrefSize.x() - ((childPrefSize.x() - childMinSize.x()) * shrinkPercent);
                    break;
                case SizeHint::EXPANDING_SHRINKING:
                    if (shrinkPercent > 0) {
                        childSize.x() = childPrefSize.x() - ((childPrefSize.x() - childMinSize.x()) * shrinkPercent);
                    } else if (expandSizePerChild > 0) {
                        childSize.x() = childPrefSize.x() + expandSizePerChild;
                    } else {
                        childSize.x() = childPrefSize.x();
                    }
                    break;
            }

            x_offset += childSize.x();

            child->set_actual_size(childSize); // triggers layout of child
        }
    }

    gml::Vec2f HListBox::preferred_size()
    {
        return gml::Vec2f(calc_pref_size_x(), calc_pref_size_y()) + gml::Vec2f(m_template.border.thickness * 2);
    }

    float HListBox::calc_pref_size_x()
    {
        float preferred_size = 0.0f;

        for (auto& child: m_children) {
            switch (child->size_hint().x()) {
                case SizeHint::FIXED:
                case SizeHint::EXPANDING:
                case SizeHint::FIT_CHILDREN:
                    preferred_size += child->preferred_size().x();
                    break;
                case SizeHint::SHRINKING:
                case SizeHint::EXPANDING_SHRINKING:
                    preferred_size = child->min_size().x();
                    break;
            }
        }

        return preferred_size;
    }

    float HListBox::calc_pref_size_y()
    {
        float preferred_size = 0.0f;

        for (auto& child: m_children) {
            switch (child->size_hint().y()) {
                case SizeHint::FIXED:
                case SizeHint::EXPANDING:
                case SizeHint::FIT_CHILDREN:
                    preferred_size = std::max(preferred_size, child->preferred_size().y());
                    break;
                case SizeHint::SHRINKING:
                case SizeHint::EXPANDING_SHRINKING:
                    preferred_size = std::max(preferred_size, child->min_size().y());
                    break;
            }
        }

        return preferred_size;
    }

    HListBox::HListBox(Widget* parent, Master* master, const WidgetTemplate& widget_template) : Widget(parent, master,
                                                                                                       widget_template)
    {}
}
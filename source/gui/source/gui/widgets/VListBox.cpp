#include "VListBox.h"

namespace gui
{
    void VListBox::update_layout()
    {
        gml::Vec2f remaining_size = m_inner_size_abs;
        float totalShrinkingSize = 0.0f;
        int nExpanding = 0;

        for (auto& child : m_children) {
            remaining_size.y() -= child->preferred_size().y();

            if (child->size_hint().y() == SizeHint::SHRINKING || child->size_hint().y() == SizeHint::EXPANDING_SHRINKING) {
                totalShrinkingSize += child->preferred_size().y() - child->min_size().y();
            }

            if (child->size_hint().y() == SizeHint::EXPANDING || child->size_hint().y() == SizeHint::EXPANDING_SHRINKING) {
                ++nExpanding;
            }
        }

        float shrinkPercent = 0.0f;
        float expandSize = 0.0f;
        if (remaining_size.y() < 0) {
            shrinkPercent = (-1 * remaining_size.y()) / totalShrinkingSize; // TODO: division by zero / negative numbers
        } else if (remaining_size.y() > 0) {
            expandSize = remaining_size.y() / nExpanding;
        }

        float y_offset = 0;
        for (auto& child: m_children) {
            gml::Vec2f childPrefSize = child->preferred_size();
            gml::Vec2f childMinSize = child->min_size();
            gml::Vec2f childSize(0.0f);

            child->override_anchor({.position = AnchorPosition::TOP_LEFT});
            child->set_absolute_offset({0, y_offset});

            switch (child->size_hint().x()) {
                case SizeHint::FIXED:
                case SizeHint::FIT_CHILDREN:
                    childSize.x() = childPrefSize.x();
                    break;
                case SizeHint::EXPANDING:// TODO: these should be min/max
                case SizeHint::SHRINKING:// TODO: these should be min/max
                case SizeHint::EXPANDING_SHRINKING:// TODO: these should be min/max
                    childSize.x() = m_inner_size_abs.x();
                    break;
            }

            switch (child->size_hint().y()) {
                case SizeHint::FIXED:
                case SizeHint::FIT_CHILDREN:
                    childSize.y() = childPrefSize.y();
                    break;
                case SizeHint::EXPANDING:
                    childSize.y() = childPrefSize.y() + expandSize;
                    break;
                case SizeHint::SHRINKING:
                    childSize.y() = childPrefSize.y() - ((childPrefSize.y() - childMinSize.y()) * shrinkPercent);
                    break;
                case SizeHint::EXPANDING_SHRINKING:
                    if (shrinkPercent > 0) {
                        childSize.y() = childPrefSize.y() - ((childPrefSize.y() - childMinSize.y()) * shrinkPercent);
                    } else if (expandSize > 0) {
                        childSize.y() = childPrefSize.y() + expandSize;
                    } else {
                        childSize.y() = childPrefSize.y();
                    }
                    break;
            }

            y_offset += childSize.y();

            child->set_actual_size(childSize);
            child->update_layout();
        }
    }

    gml::Vec2f VListBox::preferred_size() const
    {
        return gml::Vec2f(calc_pref_size_x(), calc_pref_size_y()) + gml::Vec2f(m_template.border.thickness * 2);
    }

    float VListBox::calc_pref_size_x() const
    {
        float preferred_size = 0.0f;

        for (auto& child : m_children) {
            switch (child->size_hint().x()) {
                case SizeHint::FIXED:
                case SizeHint::EXPANDING:
                case SizeHint::FIT_CHILDREN:
                    preferred_size = std::max(preferred_size, child->preferred_size().x());
                    break;
                case SizeHint::SHRINKING:
                case SizeHint::EXPANDING_SHRINKING:
                    preferred_size = std::max(preferred_size, child->min_size().x());
                    break;
            }
        }

        return preferred_size;
    }

    float VListBox::calc_pref_size_y() const
    {
        float preferred_size = 0.0f;

        for (auto& child: m_children) {
            switch (child->size_hint().y()) {
                case SizeHint::FIXED:
                case SizeHint::EXPANDING:
                case SizeHint::FIT_CHILDREN:
                    preferred_size += child->preferred_size().y();
                    break;
                case SizeHint::SHRINKING:
                case SizeHint::EXPANDING_SHRINKING:
                    preferred_size += child->min_size().y();
                    break;
            }
        }

        return preferred_size;
    }

    VListBox::VListBox(Widget* parent, Master* master, const WidgetTemplate& widget_template) : Widget(parent, master,
                                                                                                       widget_template)
    {}
}
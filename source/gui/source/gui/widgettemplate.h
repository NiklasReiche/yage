#pragma once

#include "gml/vector.h"
#include "core/gl/color.h"
#include <core/gl/graphics.h>

namespace gui
{
    enum class SizeHint
    {
        /** This element must stay at a fixed size */
        FIXED,
        /** This element can expand */
        EXPANDING,
        /** This element can shrink */
        SHRINKING,
        /** This element can expand and shrink */
        EXPANDING_SHRINKING,
        /** This element must retain its aspect ratio */
        //ASPECT, // TODO
        /** This element must fit its children */
        FIT_CHILDREN
    };

    enum class ValueType
    {
        ABSOLUTE,
        RELATIVE_TO_PARENT
    };

    struct Size
    {
        gml::Vec2f value = gml::Vec2f(0);
        gml::Vec2<ValueType> value_type = {ValueType::ABSOLUTE, ValueType::ABSOLUTE};
    };

    enum class AnchorPosition
    {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        CENTER
    };

    struct Anchor
    {
        /**
         * Position within the parent from which to offset the child. Also encodes the reference edge on the child.
         */
        AnchorPosition position = AnchorPosition::TOP_LEFT;

        /**
         * Offset from the anchor position inside the parent widget, either in absolute or relative terms.
         */
        gml::Vec2f offset = gml::Vec2f(0);

        /**
         * Encodes whether the provided offset values are absolute or relative.
         */
        gml::Vec2<ValueType> value_type = {ValueType::ABSOLUTE, ValueType::ABSOLUTE};

        bool operator==(const Anchor& rhs) const
        {
            return offset == rhs.offset &&
                   value_type == rhs.value_type &&
                   position == rhs.position;
        }

        bool operator!=(const Anchor& rhs) const
        {
            return !(rhs == *this);
        }
    };

    struct WidgetGeometryTemplate
    {
        /** how the widget is anchored and offset within its parent */
        Anchor anchor{};
        /** preferred size for layouts */
        Size preferred_size{};
        gml::Vec2<SizeHint> size_hint = {SizeHint::FIT_CHILDREN, SizeHint::FIT_CHILDREN};
        /** minimal size for layouts */
        Size min_size{}; // TODO: does it make sense for this to be relative?
        /** maximum size for layouts */
        Size max_size{}; // TODO: does it make sense for this to be relative?
    };

    struct WidgetBorderTemplate
    {
        float thickness = 0.0f;
        gl::Color_t color = gl::Color::BLACK;
    };

    struct WidgetShadowTemplate
    {
        float offset = 0.0f;
        float hardness = 0.5f;
    };

    struct WidgetTextureTemplate
    {
        std::string filename;
        float resizeFactor = 1.0f;
    };

    struct WidgetTemplate
    {
        WidgetGeometryTemplate geometry{};
        WidgetBorderTemplate border{};
        WidgetShadowTemplate shadow{};
        unsigned int color = gl::Color::TRANSPARENT;
        WidgetTextureTemplate texture{};
    };
}
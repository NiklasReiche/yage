#pragma once

#include <string>

#include "widget.h"

namespace gui
{
    enum class TextAlignmentY
    {
        TOP,
        BOTTOM,
        CENTER
    };

    enum class TextAlignmentX
    {
        LEFT,
        RIGHT,
        CENTER
    };

    struct TextTemplate
    {
        std::u32string text;
        res::Resource<font::Font> font; // TODO: provide a default font somewhere
        int size = 16;
        gl::Color_t color = gl::Color::BLACK;
        TextAlignmentX align_x = TextAlignmentX::LEFT;
        TextAlignmentY align_y = TextAlignmentY::TOP;
    };

    struct LabelTemplate
    {
        WidgetTemplate base{};
        TextTemplate text;
    };

    /**
     * A widget that displays text.
     */
    class Label : public Widget
    {
    public:
        Label(Widget* parent, Master* master, const LabelTemplate& label_template);

        void update_geometry() override;

        [[nodiscard]]
        gml::Vec2f preferred_size() const override;

        [[nodiscard]]
        font::Text* text() const override;

        void set_text(const TextTemplate& text);

        void set_text(const std::u32string& string);

    private:
        LabelTemplate m_label_template;
        std::unique_ptr<font::Text> m_text;
    };
}
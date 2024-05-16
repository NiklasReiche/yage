#include "label.h"
#include "../master.h"

namespace gui
{
    Label::Label(Widget* parent, Master* master, const LabelTemplate& label_template)
            : Widget(parent, master, label_template.base), m_label_template(label_template)
    {
        m_label_template = label_template;

        if (!label_template.text.text.empty()) {
            gml::Vec2f textPosition = m_inner_position_abs + label_template.padding;
            m_text = std::make_unique<font::Text>(master->gl_context().getDrawableCreator(),
                                                  label_template.text.text, label_template.text.font,
                                                  font::TextParameters{
                                                          .color = label_template.text.color,
                                                          .pt_size = (float) label_template.text.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
            );
        }

        // TODO
        if (m_template.geometry.preferred_size.value == gml::Vec2f(0.0f)) {
            m_template.geometry.size_hint = gml::Vec2<SizeHint>(SizeHint::FIT_CHILDREN);
        }
    }

    gml::Vec2f Label::preferred_size() const
    {
        // TODO
        gml::Vec2f size = m_template.geometry.preferred_size.value;
        if (m_template.geometry.size_hint.x() == SizeHint::FIT_CHILDREN && m_text != nullptr) {
            size.x() = m_text->dimensions().x() + (float) m_template.border.thickness * 2 +
                       m_label_template.padding.x() * 2;
        }
        if (m_template.geometry.size_hint.y() == SizeHint::FIT_CHILDREN && m_text != nullptr) {
            size.y() = m_text->max_font_dimensions().y() + (float) m_template.border.thickness * 2 +
                       m_label_template.padding.y() * 2;
        }

        return size;
    }

    void Label::set_text(const TextTemplate& text)
    {
        m_label_template.text = text;

        if (m_label_template.text.text.empty()) {
            m_text = nullptr;
        } else {
            gml::Vec2f textPosition = m_inner_position_abs + m_label_template.padding;
            m_text = std::make_unique<font::Text>(m_master->gl_context().getDrawableCreator(),
                                                  text.text, text.font,
                                                  font::TextParameters{
                                                          .color = text.color,
                                                          .pt_size = (float) text.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
            );
        }

        m_parent->update_layout();
    }

    void Label::set_text(const std::u32string& string)
    {
        m_label_template.text.text = string;
        set_text(m_label_template.text);
    }


    void Label::update_geometry()
    {
        Widget::update_geometry();
        if (m_text == nullptr) {
            return;
        }

        // TODO: somethings wrong, maybe for negative available size
        gml::Vec2f availableSize = m_inner_size_abs - m_label_template.padding - m_text->dimensions();
        gml::Vec2f textPosition(0.0f);
        switch (m_label_template.text.align_x) {
            case TextAlignmentX::LEFT:
                textPosition.x() = m_inner_position_abs.x() + m_label_template.padding.x();
                break;
            case TextAlignmentX::RIGHT:
                textPosition.x() = m_inner_position_abs.x() + availableSize.x();
                break;
            case TextAlignmentX::CENTER:
                textPosition.x() = m_inner_position_abs.x() + availableSize.x() / 2;
                break;
        }
        switch (m_label_template.text.align_y) {
            case TextAlignmentY::TOP:
                textPosition.y() = m_inner_position_abs.y() + m_label_template.padding.y();
                break;
            case TextAlignmentY::BOTTOM:
                textPosition.y() = m_inner_position_abs.y() + availableSize.y();
                break;
            case TextAlignmentY::CENTER:
                textPosition.y() = m_inner_position_abs.y() + availableSize.y() / 2;
                break;
        }

        m_text->update_offset(gml::Vec2f{textPosition.x(), textPosition.y()});
    }

    font::Text* Label::text() const
    {
        return m_text.get();
    }
}
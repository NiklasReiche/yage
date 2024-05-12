#include "label.h"
#include "../master.h"

namespace gui
{
    Label::Label(Widget* parent, Master* master, LabelTemplate labelTemplate)
            : Widget(parent, master, labelTemplate.base), params(labelTemplate), padding(labelTemplate.padding)
    {
        this->m_has_text = true;

        alignX = labelTemplate.text.alignX;
        alignY = labelTemplate.text.alignY;

        if (labelTemplate.text.text.empty())
        {
            labelTemplate.text.text = U" ";
        }
        params = labelTemplate;

        gml::Vec2f textPosition = this->m_inner_position_abs + padding;
        m_text = std::make_unique<font::Text>(master->gl_context().getDrawableCreator(),
                                                  labelTemplate.text.text, labelTemplate.text.font,
                                                  font::TextParameters{
                                                          .color = labelTemplate.text.color,
                                                          .pt_size = (float) labelTemplate.text.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
        );

        // TODO
        if (m_template.geometry.preferred_size.value == gml::Vec2f(0.0f))
        {
            m_template.geometry.size_hint = gml::Vec2<SizeHint>(SizeHint::FIT_CHILDREN);
        }
    }

    gml::Vec2f Label::preferred_size()
    {
        return {
            m_text->dimensions().x() + (float) m_template.border.thickness * 2 + padding.x() * 2,
            m_text->max_font_dimensions().y() + (float) m_template.border.thickness * 2 + padding.y() * 2
        };
    }

    void Label::setText(TextTemplate textTemplate)
    {
        if (textTemplate.text.empty())
        {
            textTemplate.text = U" ";
        }

        params.text = textTemplate;

        gml::Vec2f textPosition = m_inner_position_abs + padding;
        m_text = std::make_unique<font::Text>(m_master->gl_context().getDrawableCreator(),
                                              textTemplate.text, textTemplate.font,
                                              font::TextParameters{
                                                          .color = textTemplate.color,
                                                          .pt_size = (float) textTemplate.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
        );

        m_parent->update_layout();
    }

    void Label::setText(const std::u32string &string)
    {
        params.text.text = string;
        setText(params.text);
    }


    void Label::update_geometry()
    {
        Widget::update_geometry();

        // TODO: somethings wrong, maybe for negative available size
        gml::Vec2f availableSize = m_inner_size_abs - padding - m_text->dimensions();
        gml::Vec2f textPosition(0.0f);

        switch (alignX)
        {
            case TextAlignmentX::LEFT:
                textPosition.x() = m_inner_position_abs.x() + padding.x();
                break;
            case TextAlignmentX::RIGHT:
                textPosition.x() = m_inner_position_abs.x() + availableSize.x();
                break;
            case TextAlignmentX::CENTER:
                textPosition.x() = m_inner_position_abs.x() + availableSize.x() / 2;
                break;
        }
        switch (alignY)
        {
            case TextAlignmentY::TOP:
                textPosition.y() = m_inner_position_abs.y() + padding.y();
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
}
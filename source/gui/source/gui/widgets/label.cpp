#include "label.h"
#include "../master.h"

namespace gui
{
    Label::Label(Widget* parent, Master* master, LabelTemplate labelTemplate)
            : Widget(parent, master, labelTemplate), params(labelTemplate), padding(labelTemplate.padding)
    {
        this->m_has_text = true;

        alignX = labelTemplate.text.alignX;
        alignY = labelTemplate.text.alignY;

        if (labelTemplate.text.text.length() == 0)
        {
            labelTemplate.text.text = U" ";
        }

        gml::Vec2f textPosition = this->m_inner_position + padding;
        m_text = std::make_unique<font::Text>(master->gl_context().getDrawableCreator(),
                                                  labelTemplate.text.text, labelTemplate.text.font,
                                                  font::TextParameters{
                                                          .color = labelTemplate.text.color,
                                                          .pt_size = (float) labelTemplate.text.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
        );

        if (m_hint_pref_size == gml::Vec2f(0.0f))
        {
            m_fit_children = true;
            m_hint_pref_size = calcPrefSize();
            m_size_hint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
        }
    }

    gml::Vec2f Label::calcPrefSize()
    {
        gml::Vec2f vec;
        vec.x() = m_text->dimensions().x() + (float) this->m_border_size * 2 + padding.x() * 2;
        vec.y() = m_text->max_font_dimensions().y() + (float) this->m_border_size * 2 + padding.y() * 2;
        return vec;
    }

    void Label::setText(TextTemplate textTemplate)
    {
        if (textTemplate.text.empty())
        {
            textTemplate.text = U" ";
        }

        params.text = textTemplate;

        gml::Vec2f textPosition = m_inner_position + padding;
        m_text = std::make_unique<font::Text>(m_master->gl_context().getDrawableCreator(),
                                              textTemplate.text, textTemplate.font,
                                              font::TextParameters{
                                                          .color = textTemplate.color,
                                                          .pt_size = (float) textTemplate.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
        );

        if (m_fit_children)
        {
            m_hint_pref_size = calcPrefSize();
            m_parent->update_layout();
        }
    }

    void Label::setText(const std::u32string &string)
    {
        params.text.text = string;
        setText(params.text);
    }


    void Label::update_geometry()
    {
        Widget::update_geometry();

        gml::Vec2f availableSize = m_inner_size - padding - m_text->dimensions();
        gml::Vec2f textPosition(0.0f);

        switch (alignX)
        {
            case TextAlignmentX::LEFT:
                textPosition.x() = m_inner_position.x() + padding.x();
                break;
            case TextAlignmentX::RIGHT:
                textPosition.x() = m_inner_position.x() + availableSize.x();
                break;
            case TextAlignmentX::CENTER:
                textPosition.x() = m_inner_position.x() + availableSize.x() / 2;
                break;
        }
        switch (alignY)
        {
            case TextAlignmentY::TOP:
                textPosition.y() = m_inner_position.y() + padding.y();
                break;
            case TextAlignmentY::BOTTOM:
                textPosition.y() = m_inner_position.y() + availableSize.y();
                break;
            case TextAlignmentY::CENTER:
                textPosition.y() = m_inner_position.y() + availableSize.y() / 2;
                break;
        }

        m_text->update_offset(gml::Vec2f{textPosition.x(), textPosition.y()});
    }
}
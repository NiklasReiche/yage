#include "label.h"
#include "../master.h"

namespace gui
{
    Label::Label(Widget* parent, Master* master, LabelTemplate labelTemplate)
            : Widget(parent, master, labelTemplate), params(labelTemplate), padding(labelTemplate.padding)
    {
        this->hasText = true;

        alignX = labelTemplate.text.alignX;
        alignY = labelTemplate.text.alignY;

        if (labelTemplate.text.text.length() == 0)
        {
            labelTemplate.text.text = U" ";
        }

        gml::Vec2f textPosition = this->innerPosition + padding;
        this->text = std::make_unique<font::Text>(master->gl_context().getDrawableCreator(),
                                                  labelTemplate.text.text, labelTemplate.text.font,
                                                  font::TextParameters{
                                                          .color = labelTemplate.text.color,
                                                          .pt_size = (float) labelTemplate.text.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
        );

        if (prefSize == gml::Vec2f(0.0f))
        {
            fitChildren = true;
            prefSize = calcPrefSize();
            sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
        }
    }

    gml::Vec2f Label::calcPrefSize()
    {
        gml::Vec2f vec;
        vec.x() = this->text->dimensions().x() + (float) this->borderSize * 2 + padding.x() * 2;
        vec.y() = this->text->max_font_dimensions().y() + (float) this->borderSize * 2 + padding.y() * 2;
        return vec;
    }

    void Label::setText(TextTemplate textTemplate)
    {
        if (textTemplate.text.empty())
        {
            textTemplate.text = U" ";
        }

        params.text = textTemplate;

        gml::Vec2f textPosition = innerPosition + padding;
        this->text = std::make_unique<font::Text>(master->gl_context().getDrawableCreator(),
                                                  textTemplate.text, textTemplate.font,
                                                  font::TextParameters{
                                                          .color = textTemplate.color,
                                                          .pt_size = (float) textTemplate.size,
                                                          .offset = gml::Vec3f(textPosition.x(), textPosition.y(), -1)
                                                  }
        );

        if (fitChildren)
        {
            prefSize = calcPrefSize();
            parent->update_layout();
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

        gml::Vec2f availableSize = innerSize - padding - text->dimensions();
        gml::Vec2f textPosition(0.0f);

        switch (alignX)
        {
            case TextAlignmentX::LEFT:
                textPosition.x() = innerPosition.x() + padding.x();
                break;
            case TextAlignmentX::RIGHT:
                textPosition.x() = innerPosition.x() + availableSize.x();
                break;
            case TextAlignmentX::CENTER:
                textPosition.x() = innerPosition.x() + availableSize.x() / 2;
                break;
        }
        switch (alignY)
        {
            case TextAlignmentY::TOP:
                textPosition.y() = innerPosition.y() + padding.y();
                break;
            case TextAlignmentY::BOTTOM:
                textPosition.y() = innerPosition.y() + availableSize.y();
                break;
            case TextAlignmentY::CENTER:
                textPosition.y() = innerPosition.y() + availableSize.y() / 2;
                break;
        }

        text->update_offset(gml::Vec2f{textPosition.x(), textPosition.y()});
    }
}
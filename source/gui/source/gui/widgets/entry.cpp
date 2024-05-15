#include "entry.h"
#include "utils/strings.h"
#include <core/platform/Window.h>
#include "../master.h"

namespace gui
{
    TextCursor::TextCursor(Widget* parent, Master* master, WidgetTemplate widgetTemplate)
            : Widget(parent, master, widgetTemplate)
    {
        m_template.geometry.size_hint.x() = SizeHint::FIXED;
        m_template.geometry.size_hint.y() = SizeHint::FIXED;
    }

    CursorAnimation::CursorAnimation(Widget* widget, Master* master, double time)
            : Animation(widget, master, gml::Vec2f(), gml::Vec2f(), time)
    {

    }

    void CursorAnimation::update(double dt)
    {
        if (!isFinished) {
            if (timeCurrent < timeEnd) {
                timeCurrent += dt;
            } else {
                if (widget->is_Active()) {
                    widget->hide();
                } else {
                    widget->show();
                }
                timeCurrent = 0.0;
            }
        }
    }

    void TextEntry::moveCursor()
    {
        gml::Vec2f cursor_position;
        cursor_position.x() = label->offset().x() + label->text()->relative_offset(cursorPosition).x();
        cursor_position.y() = cursor->offset().y();
        cursor->set_anchor({
                                   .position = AnchorPosition::TOP_LEFT,
                                   .offset = cursor_position,
                                   .value_type = {ValueType::ABSOLUTE, ValueType::ABSOLUTE},
                           });
    }

    TextEntry::TextEntry(Widget* parent, Master* master, TextEntryTemplate entryTemplate)
            : Widget(parent, master, entryTemplate.base),
              padding(entryTemplate.padding),
              callback(entryTemplate.command),
              cursorColor(entryTemplate.cursorColor),
              cursorWidth(entryTemplate.cursorWidth),
              defaultTextTemplate(entryTemplate.defaultText),
              inputTextTemplate(entryTemplate.inputText)
    {
        m_is_interactable = true;
        m_keep_focus = true;

        inputTextTemplate.size = defaultTextTemplate.size;
        inputTextTemplate.font = defaultTextTemplate.font;

        LabelTemplate labelTemplate{
            .text = entryTemplate.defaultText
        };
        labelTemplate.padding = gml::Vec2f(0.0f);
        label = create_widget<Label>(labelTemplate);

        WidgetTemplate cursorTemplate;
        //cursorTemplate.geometry.anchor.offset = gml::Vec2f(padding.x(), padding.y());
        cursorTemplate.geometry.preferred_size.value = gml::Vec2f(cursorWidth,
                                                                  label->preferred_size().y() - padding.y() * 2);
        cursorTemplate.color = cursorColor;

        cursor = create_widget<TextCursor>(cursorTemplate);
        cursorAnimation = cursor->create_animation<CursorAnimation>(0.5);
        cursor->hide();

        if (m_template.geometry.preferred_size.value == gml::Vec2f(0.0f)) {
            m_template.geometry.size_hint.x() = SizeHint::FIT_CHILDREN;
            m_template.geometry.size_hint.y() = SizeHint::FIT_CHILDREN;
        }
    }

    void TextEntry::on_focus()
    {
        m_master->window().enableCharInput();
        cursor->show();
        cursorAnimation->start();
        if (inputText.empty()) {
            inputTextTemplate.text = U"";
            label->setText(inputTextTemplate);
        }
    }

    void TextEntry::on_focus_release()
    {
        m_master->window().disableCharInput();
        cursor->hide();
        cursorAnimation->stop();
        if (inputText.empty()) {
            label->setText(defaultTextTemplate);
        }
    }

    void TextEntry::on_char_input(unsigned int character)
    {
        inputText.insert(inputText.begin() + cursorPosition, static_cast<char32_t>(character));
        label->setText(inputText);

        if (cursorPosition < (int) inputText.length()) {
            cursorPosition++;
            moveCursor();
        }
    }

    void TextEntry::on_key_press(input::KeyEvent::Code key)
    {
        switch (key) {
            case input::KeyEvent::Code::KEY_BACKSPACE:
                if (cursorPosition > 0) {
                    cursorPosition--;
                    inputText.erase(cursorPosition, 1);
                    label->setText(inputText);
                    moveCursor();
                }
                break;

            case input::KeyEvent::Code::KEY_LEFT:
                if (cursorPosition > 0) {
                    cursorPosition--;
                    moveCursor();
                }
                break;

            case input::KeyEvent::Code::KEY_RIGHT:
                if (cursorPosition < (int) inputText.length()) {
                    cursorPosition++;
                    moveCursor();
                }
                break;

            case input::KeyEvent::Code::KEY_ENTER:
                m_master->input_manager().unFocus();
                if (callback)
                    callback();
                break;

            default:
                break;
        }
    }

    std::u32string TextEntry::value() const
    {
        return inputText;
    }
}
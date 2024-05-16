#include "entry.h"
#include "utils/strings.h"
#include <core/platform/Window.h>
#include "../master.h"

namespace gui
{
    TextCursor::TextCursor(Widget* parent, Master* master, const WidgetTemplate& widget_template)
            : Widget(parent, master, widget_template)
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

    TextEntry::TextEntry(Widget* parent, Master* master, const TextEntryTemplate& entry_template)
            : Widget(parent, master, entry_template.base),
              m_entry_template(entry_template)
    {
        m_is_interactable = true;
        m_keep_focus = true;

        LabelTemplate labelTemplate{
                .text = entry_template.placeholder_text
        };
        labelTemplate.padding = gml::Vec2f(0.0f);
        label = create_widget<Label>(labelTemplate);

        WidgetTemplate cursorTemplate;
        //cursorTemplate.geometry.anchor.offset = gml::Vec2f(padding.x(), padding.y());
        cursorTemplate.geometry.preferred_size.value = gml::Vec2f(CURSOR_WIDTH,
                                                                  label->preferred_size().y() -
                                                                  m_entry_template.padding.y() * 2);
        cursorTemplate.color = m_entry_template.cursor_color;

        cursor = create_widget<TextCursor>(cursorTemplate);
        cursor_animation = cursor->create_animation<CursorAnimation>(0.5);
        cursor->hide();

        if (m_template.geometry.preferred_size.value == gml::Vec2f(0.0f)) {
            m_template.geometry.size_hint.x() = SizeHint::FIT_CHILDREN;
            m_template.geometry.size_hint.y() = SizeHint::FIT_CHILDREN;
        }
    }

    void TextEntry::move_cursor(std::size_t position)
    {
        gml::Vec2f cursor_position;
        cursor_position.x() = label->offset().x() + (label->text() ? label->text()->relative_offset(position).x() : 0);
        cursor_position.y() = cursor->offset().y();
        cursor->set_anchor({
                                   .position = AnchorPosition::TOP_LEFT,
                                   .offset = cursor_position,
                                   .value_type = {ValueType::ABSOLUTE, ValueType::ABSOLUTE},
                           });
    }

    void TextEntry::on_focus()
    {
        m_master->window().enableCharInput();
        cursor->show();
        cursor_animation->start();
        if (m_input_text.empty()) {
            label->set_text(m_entry_template.input_text);
        }
    }

    void TextEntry::on_focus_release()
    {
        m_master->window().disableCharInput();
        cursor->hide();
        cursor_animation->stop();
        if (m_input_text.empty()) {
            label->set_text(m_entry_template.placeholder_text);
        }
    }

    void TextEntry::on_char_input(unsigned int character)
    {
        m_input_text.insert(m_cursor_position, 1, static_cast<char32_t>(character));
        label->set_text(m_input_text);
        move_cursor(++m_cursor_position);
    }

    void TextEntry::on_key_press(input::KeyEvent::Code key)
    {
        switch (key) {
            case input::KeyEvent::Code::KEY_BACKSPACE:
                if (m_cursor_position > 0) {
                    m_input_text.erase(--m_cursor_position, 1);
                    label->set_text(m_input_text);
                    move_cursor(m_cursor_position);
                }
                break;

            case input::KeyEvent::Code::KEY_DELETE:
                if (m_cursor_position > 0) {
                    m_input_text.erase(m_cursor_position, 1);
                    label->set_text(m_input_text);
                    move_cursor(m_cursor_position);
                }
                break;

            case input::KeyEvent::Code::KEY_LEFT:
                if (m_cursor_position > 0) {
                    move_cursor(--m_cursor_position);
                }
                break;

            case input::KeyEvent::Code::KEY_RIGHT:
                if (m_cursor_position < m_input_text.length()) {
                    move_cursor(++m_cursor_position);
                }
                break;

            case input::KeyEvent::Code::KEY_END:
                if (m_cursor_position < m_input_text.length()) {
                    m_cursor_position = m_input_text.length();
                    move_cursor(m_cursor_position);
                }
                break;

            case input::KeyEvent::Code::KEY_HOME:
                if (m_cursor_position > 0) {
                    m_cursor_position = 0;
                    move_cursor(m_cursor_position);
                }
                break;

            case input::KeyEvent::Code::KEY_ENTER:
                m_master->input_manager().release_focus();
                if (m_entry_template.command)
                    m_entry_template.command();
                break;

            default:
                break;
        }
    }

    std::u32string TextEntry::value() const
    {
        return m_input_text;
    }
}
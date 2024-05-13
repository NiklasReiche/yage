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
			}
			else {
				if (widget->is_Active()) {
					widget->hide();
				}
				else {
					widget->show();
				}
				timeCurrent = 0.0;
			}
		}
	}

	void TextEntry::moveCursor()
	{
		gml::Vec2f cursor_position;
        cursor_position.x() = label->offset().x() + label->text()->offset(cursorPosition).x();
        cursor_position.y() = cursor->offset().y();
		cursor->set_anchor({
            .offset = cursor_position,
            .value_type = {ValueType::ABSOLUTE, ValueType::ABSOLUTE},
            .position = AnchorPosition::TOP_LEFT
        });
	}

	TextEntry::TextEntry(Widget * parent, Master* master, TextEntryTemplate entryTemplate, Master* m)
		: Widget(parent, master, entryTemplate), 
		padding(entryTemplate.padding),
		callback(entryTemplate.command),
		cursorColor(entryTemplate.cursorColor),
		cursorWidth(entryTemplate.cursorWidth),
		defaultTextTemplate(entryTemplate.defaultText),
		inputTextTemplate(entryTemplate.inputText)
	{
		this->m_is_interactable = true;
		this->m_keep_focus = true;

		inputTextTemplate.size = defaultTextTemplate.size;
		inputTextTemplate.font = defaultTextTemplate.font;

		LabelTemplate labelTemplate{
            .text = entryTemplate.defaultText
        };
		labelTemplate.base.geometry.anchor.offset = entryTemplate.padding;
		labelTemplate.base.geometry.preferred_size.value = gml::Vec2f();
		labelTemplate.base.color = 0x00000000u;
		labelTemplate.base.shadow.offset = 0;
		labelTemplate.base.border.thickness = 0;
		labelTemplate.padding = gml::Vec2f(0.0f);

		label = this->create_widget<Label>(labelTemplate);

		WidgetTemplate cursorTemplate;
		cursorTemplate.geometry.anchor.offset = gml::Vec2f(padding.x(), padding.y());
		cursorTemplate.geometry.preferred_size.value = gml::Vec2f(cursorWidth, label->preferred_size().y() - padding.y() * 2);
		cursorTemplate.color = cursorColor;

		cursor = this->create_widget<TextCursor>(cursorTemplate);
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
		if (inputText.length() == 0) {
			inputTextTemplate.text = U"";
			label->setText(inputTextTemplate);
		}
	}
	void TextEntry::on_focus_release()
	{
		m_master->window().disableCharInput();
		cursor->hide();
		cursorAnimation->stop();
		if (inputText.length() == 0) {
			label->setText(defaultTextTemplate);
		}
	}

	void TextEntry::on_char_input(char character)
	{
		inputText.insert(cursorPosition, std::string(1, character));
		label->setText(utils::toUTF32(inputText));

		if (cursorPosition < (int)inputText.length()) {
			cursorPosition++;
			moveCursor();
		}
	}
	void TextEntry::on_key_press(input::KeyEvent::Code key)
	{
		switch (key)
		{
		case input::KeyEvent::Code::KEY_BACKSPACE:
			if (cursorPosition > 0) {
				cursorPosition--;
				this->inputText.erase(cursorPosition, 1);
				label->setText(utils::toUTF32(inputText));
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
			if (cursorPosition < (int)inputText.length()) {
				cursorPosition++;
				moveCursor();
			}
			break;

		case input::KeyEvent::Code::KEY_ENTER:
			m_master->input_manager().unFocus();
			try {
				callback();
			} catch (std::bad_function_call){}
			break;
		}
	}

	std::string TextEntry::getString()
	{
		return inputText;
	}

	gml::Vec2f TextEntry::calcPrefSize()
	{
		return label->preferred_size() + padding * 2;
	}
}
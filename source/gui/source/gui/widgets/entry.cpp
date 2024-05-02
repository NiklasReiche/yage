#include "entry.h"
#include "utils/strings.h"
#include <core/platform/Window.h>

namespace gui
{
	TextCursor::TextCursor(Widget* parent, MasterInterface master, WidgetTemplate widgetTemplate)
		: Widget(parent, master, widgetTemplate)
	{
		sizeHint.x() = SizeHint::FIXED;
		sizeHint.y() = SizeHint::FIXED;
		prefSize = widgetTemplate.geometry.size;
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
		gml::Vec2f cursorOffset;
		cursorOffset.x() = label->getOffset().x() + label->getText()->getOffset(cursorPosition).x();
		cursorOffset.y() = cursor->getOffset().y();
		cursor->move(cursorOffset);
	}

	TextEntry::TextEntry(Widget * parent, MasterInterface master, TextEntryTemplate entryTemplate, Master* m)
		: Widget(parent, master, entryTemplate), 
		padding(entryTemplate.padding),
		callback(entryTemplate.command),
		cursorColor(entryTemplate.cursorColor),
		cursorWidth(entryTemplate.cursorWidth),
		defaultTextTemplate(entryTemplate.defaultText),
		inputTextTemplate(entryTemplate.inputText)
	{
		this->isInteractable = true;
		this->keepFocus = true;

		inputTextTemplate.size = defaultTextTemplate.size;
		inputTextTemplate.font = defaultTextTemplate.font;

		LabelTemplate labelTemplate{
            .text = entryTemplate.defaultText
        };
		labelTemplate.geometry.offset = entryTemplate.padding;
		labelTemplate.geometry.size = gml::Vec2f();
		labelTemplate.color = 0x00000000u;
		labelTemplate.shadow.offset = 0;
		labelTemplate.border.size = 0;
		labelTemplate.padding = gml::Vec2f(0.0f);

		label = this->createWidget<Label>(master, labelTemplate);

		WidgetTemplate cursorTemplate;
		cursorTemplate.geometry.offset = gml::Vec2f(padding.x(), padding.y());
		cursorTemplate.geometry.size = gml::Vec2f(cursorWidth, label->getPreferredSize().y() - padding.y() * 2);
		cursorTemplate.color = cursorColor;

		cursor = this->createWidget<TextCursor>(master, cursorTemplate);
		cursorAnimation = cursor->createAnimation<CursorAnimation>(m, 0.5);
		cursor->hide();

		layout = std::make_unique<AbsoluteLayout>();

		if (prefSize == gml::Vec2f(0.0f)) {
			prefSize = calcPrefSize();
			sizeHint.x() = SizeHint::EXPANDING;
			sizeHint.y() = SizeHint::FIXED;
		}
	}

	void TextEntry::onFocus()
	{
		master.platform->enableCharInput();
		cursor->show();
		cursorAnimation->start();
		if (inputText.length() == 0) {
			inputTextTemplate.text = U"";
			label->setText(inputTextTemplate);
		}
	}
	void TextEntry::onFocusRelease()
	{
		master.platform->disableCharInput();
		cursor->hide();
		cursorAnimation->stop();
		if (inputText.length() == 0) {
			label->setText(defaultTextTemplate);
		}
	}

	void TextEntry::onCharInput(char character)
	{
		inputText.insert(cursorPosition, std::string(1, character));
		label->setText(utils::toUTF32(inputText));

		if (cursorPosition < (int)inputText.length()) {
			cursorPosition++;
			moveCursor();
		}
	}
	void TextEntry::onKeyPress(input::KeyEvent::Code key)
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
			master.inputManger->unFocus();
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
		return label->getPreferredSize() + padding * 2;
	}
}
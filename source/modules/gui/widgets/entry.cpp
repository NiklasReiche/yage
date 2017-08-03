#include "Entry.h"


namespace gui
{
	TextCursor::TextCursor(Widget* parent, MasterInterface master, WidgetTemplate widgetTemplate)
		: Widget(parent, master, widgetTemplate)
	{
		childSizeHint.x = ChildSizeHint::FIXED;
		childSizeHint.y = ChildSizeHint::MIN_EXPAND;
		prefSize = widgetTemplate.geometry.size;
	}

	void TextEntry::moveCursor()
	{
		gml::Vec2f cursorOffset;
		cursorOffset.x = label->getOffset().x + label->getText()->getOffset(cursorPosition).x;
		cursorOffset.y = cursor->getOffset().y;
		cursor->move(cursorOffset);
	}

	TextEntry::TextEntry(Widget * parent, MasterInterface master, TextEntryTemplate entryTemplate)
		: Widget(parent, master, entryTemplate), 
		defaultTextTemplate(entryTemplate.defaultText), 
		inputTextTemplate(entryTemplate.inputText),
		padding(entryTemplate.padding),
		callback(entryTemplate.command),
		cursorWidth(entryTemplate.cursorWidth),
		cursorColor(entryTemplate.cursorColor)
	{
		this->isInteractable = true;
		this->keepFocus = true;

		inputTextTemplate.size = defaultTextTemplate.size;
		inputTextTemplate.font = defaultTextTemplate.font;

		LabelTemplate labelTemplate;
		labelTemplate.geometry.offset = entryTemplate.padding;
		labelTemplate.geometry.size = gml::Vec2f();
		labelTemplate.color = 0x00000000u;
		labelTemplate.shadow.offset = 0;
		labelTemplate.border.size = 0;
		labelTemplate.padding = gml::Vec2f(0.0f);
		labelTemplate.text = entryTemplate.defaultText;

		label = this->createWidget<Label>(master, labelTemplate);

		WidgetTemplate cursorTemplate;
		cursorTemplate.geometry.offset = gml::Vec2f(padding.x, padding.y);
		cursorTemplate.geometry.size = gml::Vec2f(cursorWidth, label->getPreferredSize().y - padding.y * 2);
		cursorTemplate.color = cursorColor;

		cursor = this->createWidget<TextCursor>(master, cursorTemplate);
		cursor->hide();

		layout = std::make_unique<AbsoluteLayout>();
		parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_CHILDREN_FIXED);
		childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
		prefSize = label->getPreferredSize() + padding * 2;
	}

	void TextEntry::onFocus()
	{
		master.platform->enableCharInput();
		cursor->show();
		if (inputText.length() == 0) {
			inputTextTemplate.text = "";
			label->setText(inputTextTemplate);
		}
	}
	void TextEntry::onFocusRelease()
	{
		master.platform->disableCharInput();
		cursor->hide();
		if (inputText.length() == 0) {
			label->setText(defaultTextTemplate);
		}
	}

	void TextEntry::onCharInput(char character)
	{
		inputText.insert(cursorPosition, std::string(1, character));
		label->setText(inputText);

		if (cursorPosition < inputText.length()) {
			cursorPosition++;
			moveCursor();
		}

		prefSize.x = label->getPreferredSize().x;
		parent->relayout();
	}
	void TextEntry::onKeyPress(input::KeyCode key)
	{
		switch (key)
		{
		case input::KeyCode::KEY_BACKSPACE:
			if (cursorPosition > 0) {
				cursorPosition--;
				this->inputText.erase(cursorPosition, 1);
				label->setText(inputText);
				moveCursor();
			}
			break;

		case input::KeyCode::KEY_LEFT:
			if (cursorPosition > 0) {
				cursorPosition--;
				moveCursor();
			}
			break;

		case input::KeyCode::KEY_RIGHT:
			if (cursorPosition < inputText.length()) {
				cursorPosition++;
				moveCursor();
			}
			break;

		case input::KeyCode::KEY_ENTER:
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
}
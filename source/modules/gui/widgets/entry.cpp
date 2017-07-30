#include "Entry.h"


namespace gui
{
	TextEntry::TextEntry(Widget * parent, MasterInterface master, TextEntryTemplate entryTemplate)
		: Widget(parent, master, entryTemplate), defaultText(entryTemplate.defaultText.text)
	{
		this->isInteractable = true;
		this->keepFocus = true;

		LabelTemplate labelTemplate;
		labelTemplate.geometry.offset = gml::Vec2f();
		labelTemplate.geometry.size = gml::Vec2f();
		labelTemplate.color = 0x00000000u;
		labelTemplate.shadow.offset = 0;
		labelTemplate.border.size = 0;
		labelTemplate.text = entryTemplate.defaultText;

		label = this->createWidget<Label>(master, labelTemplate);

		this->layout = std::make_unique<VListLayout>();
		parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_AROUND);
		childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
		prefSize = this->layout->calcParentPrefSize(this) + gml::Vec2f((float)borderSize);
	}

	void TextEntry::onFocus()
	{
		master.platform->enableCharInput();
	}
	void TextEntry::onFocusRelease()
	{
		master.platform->disableCharInput();
	}

	void TextEntry::onCharInput(char character)
	{
		text += character;
		label->setText(text);
	}
	void TextEntry::onKeyPress(input::KeyCode key)
	{
		switch (key)
		{
		case input::KeyCode::KEY_BACKSPACE:
			if (text.length() == 1) {
				text = defaultText;
			}
			else {
				text.pop_back();
			}
			label->setText(text);
			break;

		case input::KeyCode::KEY_ENTER:
			break;
		}
	}

	std::string TextEntry::getString()
	{
		return text;
	}
}
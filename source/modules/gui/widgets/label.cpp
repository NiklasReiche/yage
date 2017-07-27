#include "Label.h"

namespace gui
{	
	Label::Label(Widget * parent, MasterInterface master, LabelTemplate labelTemplate)
		: Widget(parent, master, labelTemplate), padding(labelTemplate.padding)
	{
		if (labelTemplate.text.text == "") {
			labelTemplate.text.text = " ";
		}

		this->hasText = true;
		font::Font & mFont = master.fontManager->getFont(labelTemplate.text.font);
		gml::Vec2f textPosition = this->innerPosition + padding;

		this->text = font::Text(master.glContext, labelTemplate.text.text, mFont, textPosition, labelTemplate.text.color, labelTemplate.text.size);

		if (size == gml::Vec2f(0.0f)) {
			prefSize = this->text.getSize() + gml::Vec2f(this->borderSize) * 2 + padding * 2;
		}
		else {
			prefSize = size;
		}
		parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_AROUND);
		childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
	}

	void Label::setText(TextTemplate text)
	{
		font::Font & mFont = master.fontManager->getFont(text.font);
		gml::Vec2f textPosition = this->position + gml::Vec2f(borderSize) + padding;

		this->text = font::Text(master.glContext, text.text, mFont, textPosition, text.color, text.size);

		if (size == gml::Vec2f(0.0f)) {
			prefSize = this->text.getSize() + padding * 2;
		}
		else {
			prefSize = this->text.getSize() + padding * 2;
		}
		parent->relayout();
	}


	void Label::updateGeometry()
	{
		Widget::updateGeometry();
		gml::Vec2f textPosition = position + gml::Vec2f(borderSize) + padding;
		text.setPosition(textPosition);
	}
}
#include "Label.h"

namespace gui
{	
	Label::Label(Widget * parent, MasterInterface master, LabelTemplate labelTemplate)
		: Widget(parent, master, labelTemplate), padding(labelTemplate.padding), font(labelTemplate.text.font)
	{
		this->hasText = true;

		if (labelTemplate.text.text.length() == 0) {
			labelTemplate.text.text = " ";
		}
		font::Font & mFont = master.fontManager->getFont(font);
		gml::Vec2f textPosition = this->innerPosition + padding;
		this->text = font::Text(master.glContext, labelTemplate.text.text, mFont, textPosition, labelTemplate.text.color, labelTemplate.text.size);

		if (prefSize == gml::Vec2f(0.0f)) {
			fitChildren = true;
			prefSize = calcPrefSize();
			sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
		}
	}

	gml::Vec2f Label::calcPrefSize()
	{
		gml::Vec2f vec;
		vec.x = this->text.getSize().x + (float)this->borderSize * 2 + padding.x * 2;
		vec.y = this->text.getMaxDimensions().y + (float)this->borderSize * 2 + padding.y * 2;
		return vec;
	}

	void Label::setText(TextTemplate textTemplate)
	{
		if (textTemplate.text.length() == 0) {
			textTemplate.text = " ";
		}
		font = textTemplate.font;
		font::Font & mFont = master.fontManager->getFont(font);
		gml::Vec2f textPosition = innerPosition + padding;
		this->text = font::Text(master.glContext, textTemplate.text, mFont, textPosition, textTemplate.color, textTemplate.size);

		if (fitChildren) {
			prefSize = calcPrefSize();
			parent->relayout();
		}
	}

	void Label::setText(std::string text)
	{
		font::Font & mFont = master.fontManager->getFont(font);

		if (text.length() == 0) {
			this->text.setText(" ", mFont);
		}
		else {
			this->text.setText(text, mFont);
		}

		if (fitChildren) {
			prefSize = calcPrefSize();
			parent->relayout();
		}
	}


	void Label::updateGeometry()
	{
		Widget::updateGeometry();
		gml::Vec2f textPosition = innerPosition + padding;
		text.setPosition(textPosition);
	}
}
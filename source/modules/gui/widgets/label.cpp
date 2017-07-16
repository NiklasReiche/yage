#include "Label.h"

namespace gui
{	
	Label::Label(Widget * parent, MasterInterface master, LabelLayout layout, TextLayout text)
		: Widget(parent, master, layout), padding(layout.padding)
	{
		if (text.text != "") {
			this->hasText = true;
			font::Font & mFont = master.fontManager->getFont(text.font);
			gml::Vec2f textPosition = this->position + gml::Vec2f(borderSize) + padding;

			this->text = font::Text(master.glContext, text.text, mFont, textPosition, text.color, text.size);

			if (size == gml::Vec2<float>(0.0f)) {
				resize(this->text.getSize() + padding * 2);
			}
		}
	}

	void Label::setText(TextLayout text)
	{
		font::Font & mFont = master.fontManager->getFont(text.font);
		gml::Vec2f textPosition = this->position + gml::Vec2f(borderSize) + padding;

		this->text = font::Text(master.glContext, text.text, mFont, textPosition, text.color, text.size);

		if (size == gml::Vec2f(0.0f)) {
			resize(this->text.getSize() + padding * 2);
		}
	}


	void Label::updateGeometry()
	{
		Widget::updateGeometry();
		gml::Vec2f textPosition = position + gml::Vec2f(borderSize) + padding;
		text.setPosition(textPosition);
	}
}
#include "Label.h"

namespace gui
{	
	Label::Label(Widget * parent, MasterInterface master, WidgetLayout layout, TextLayout text)
		: Widget(parent, master, layout)
	{
		if (text.text != "") {
			this->hasText = true;
			font::Font & mFont = master.fontManager->getFont(text.font);
			this->text = font::Text(master.glContext, text.text, mFont, this->position + gml::Vec2<float>(padding), text.color, text.size);

			if (size == gml::Vec2<float>(0.0f)) {
				resize(this->text.getSize() + gml::Vec2<float>(padding * 2));
			}
		}
	}

	void Label::setText(TextLayout text)
	{
		font::Font & mFont = master.fontManager->getFont(text.font);
		this->text = font::Text(master.glContext, text.text, mFont, this->position + gml::Vec2<float>(padding), text.color, text.size);

		if (size == gml::Vec2<float>(0.0f)) {
			resize(this->text.getSize() + gml::Vec2<float>(padding * 2));
		}
	}


	void Label::updateGeometry()
	{
		Widget::updateGeometry();
		text.setPosition(position);
	}
}
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
				this->size = this->text.getSize() + gml::Vec2<float>(padding * 2);
				updateParams();
			}
		}
	}

	void Label::setText(TextLayout text)
	{
		font::Font & mFont = master.fontManager->getFont(text.font);
		this->text = font::Text(master.glContext, text.text, mFont, this->position + gml::Vec2<float>(padding), text.color, text.size);

		if (size == gml::Vec2<float>(0.0f)) {
			this->size = this->text.getSize() + gml::Vec2<float>(padding * 2);
			updateParams();
			parent->setSize(this->size);
			parent->updateParams();
		}
	}

	void Label::move(gml::Vec2<float> position)
	{
		Widget::move(position);
		text.setPosition(position);
	}
}
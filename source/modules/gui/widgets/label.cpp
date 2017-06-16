#include "label.h"

namespace gui
{	
	Label::Label(Widget * parent, ManagerInterface mInterface, W_Geometry geometry, unsigned int color, W_Border border, W_Text text)
		: Widget(parent, mInterface, geometry, color, border, W_Shadow{ 0, 0.0f })
	{
		this->hasText = true;

		if (text.text != "") {
			font::Font & mFont = mInterface.fontManager->getFont(text.font);
			this->text = font::Text(mInterface.glContext, text.text, mFont, this->position + gml::Vec2<float>(padding), text.color, text.size);

			if (size == gml::Vec2<float>(0.0f)) {
				this->size = this->text.getSize() + gml::Vec2<float>(padding * 2);
				updateParams();
			}
		}
	}

	void Label::setText(W_Text text)
	{
		font::Font & mFont = masterInterface.fontManager->getFont(text.font);
		this->text = font::Text(masterInterface.glContext, text.text, mFont, this->position + gml::Vec2<float>(padding), text.color, text.size);

		if (size == gml::Vec2<float>(0.0f)) {
			this->size = this->text.getSize() + gml::Vec2<float>(padding * 2);
			updateParams();
			parent->setSize(this->size);
			parent->updateParams();
		}
	}
}
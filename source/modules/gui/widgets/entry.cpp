#include "entry.h"


namespace gui
{
	TextEntry::TextEntry(Widget * parent, ManagerInterface mInterface, W_Geometry geometry, W_Border border, unsigned int color)
		: Widget(parent, mInterface, geometry, color, border, W_Shadow{ 0, 0.0f })
	{
		geometry.position = gml::Vec2<float>();
		color = 0x00u;
		label = this->createWidget<Label>(mInterface, geometry, color, W_Border{ 0, 0x00u }, W_Text());

		if (size == gml::Vec2<float>(0.0f)) {
			this->size = label->getSize();
			updateParams();
		}
	}

	void TextEntry::onClick()
	{
		if (isFocused) {
			isFocused = false;
		}
		else {
			isFocused = true;
		}
	}

	void TextEntry::update()
	{
		//std::vector<int> & keys = masterInterface.inputManger->getPressedKeys();

		for (unsigned int i = 0; i < 1/*keys.size()*/; ++i)
		{
			//text += glfwGetKeyName(keys[i]);
		}
	}
}
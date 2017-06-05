﻿#include "fontmanager.h"

namespace gui
{
	void FontManager::addFont(std::string filename)
	{
		font::Font font = fontLoader.loadFont(filename);
		fonts[font.name] = font;
	}
	void FontManager::removeFont(std::string name)
	{
		fonts.erase(name);
	}
	font::Font& FontManager::getFont(std::string name)
	{
		return fonts.at(name);
	}
}
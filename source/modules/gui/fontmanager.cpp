#include "FontManager.h"

namespace gui
{
	FontManager::FontManager(platform::PlatformHandle* platform, gl::GraphicsContext * glContext)
		: fontLoader(font::FontLoader(platform, glContext)) {}

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
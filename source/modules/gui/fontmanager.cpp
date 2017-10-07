#include "fontmanager.h"

namespace gui
{
	FontManager::FontManager(sys::PlatformHandle* platform, gl::GraphicsContext * glContext)
		: fontLoader(font::FontLoader(platform, glContext)), platform(platform) {}

	void FontManager::addFont(std::string filename)
	{
		font::Font font = fontLoader.loadFont(filename, 16, platform->getDPI());
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
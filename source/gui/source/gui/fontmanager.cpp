#include "fontmanager.h"

namespace gui
{
	FontManager::FontManager(const gl::TextureCreator& textureCreator, const int dpi)
		: fontLoader(textureCreator), dpi(dpi) {}

	void FontManager::addFont(const std::string& filename)
	{
		const auto font = fontLoader.loadFont(filename, 16, dpi);
		fonts[font.name] = font;
	}
	void FontManager::removeFont(const std::string& name)
	{
		fonts.erase(name);
	}
	font::Font& FontManager::getFont(const std::string& name)
	{
		return fonts.at(name);
	}
}

#pragma once

#include <string>
#include <map>

#include <graphics/TextureCreator.h>
#include <font/font.h>
#include <font/loader.h>

namespace gui
{
	class FontManager
	{
	public:
		FontManager(const gl::TextureCreator& textureCreator, int dpi);

		void addFont(const std::string& filename);
		void removeFont(const std::string& name);
		font::Font& getFont(const std::string& name);

	private:
		font::FontLoader fontLoader;
		std::map<std::string, font::Font> fonts;
		int dpi;
	};
}

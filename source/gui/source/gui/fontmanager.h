#pragma once

#include <string>
#include <map>

#include <core/platform/IFileReader.h>
#include <font/font.h>
#include <font/loader.h>

namespace gui
{
	class FontManager
	{
	public:
		FontManager(const std::shared_ptr<gl::ITextureCreator> &textureCreator,
                    const std::shared_ptr<platform::IFileReader> &fileReader, int dpi);

		void addFont(const std::string &name, const std::string& filename);
		void removeFont(const std::string& name);
        std::shared_ptr<font::Font> getFont(const std::string& name);

	private:
        // TODO: better pointers
        std::shared_ptr<gl::ITextureCreator> textureCreator;
        std::shared_ptr<platform::IFileReader> fileReader;
		font::FontLoader fontLoader;
		std::map<std::string, std::shared_ptr<font::Font>> fonts;
		int dpi;
	};
}

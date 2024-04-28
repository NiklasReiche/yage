#include "fontmanager.h"

namespace gui
{
	FontManager::FontManager(const std::shared_ptr<gl::ITextureCreator> &textureCreator,
                             const std::shared_ptr<platform::IFileReader> &fileReader, int dpi)
		: textureCreator(textureCreator),
        fileReader(fileReader),
        fontLoader(font::FontLoader(textureCreator)),
        dpi(dpi) {}

	void FontManager::addFont(const std::string &name, const std::string& filename)
	{
        auto file = fileReader->openBinaryFile(filename, platform::IFile::AccessMode::READ);
        fonts[name] = fontLoader.loadFont(*file, 16, dpi);
	}

	void FontManager::removeFont(const std::string& name)
	{
		fonts.erase(name);
	}

	std::shared_ptr<font::Font> FontManager::getFont(const std::string& name)
	{
		return fonts.at(name);
	}
}

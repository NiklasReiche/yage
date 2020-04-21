#include "loader.h"
#include "fileformat.h"
#include <platform/FileLoader.h>
#include <platform/File.h>

#include <utility>

namespace font
{
	gml::Vec2<float> FontLoader::calcScale(const int ptSize, const int emSize, int dpiHorizontal, int dpiVertical)
	{
		if (dpiVertical == 0 && dpiHorizontal != 0) 
		{
			dpiVertical = dpiHorizontal;
		}
		else if (dpiHorizontal == 0 && dpiVertical != 0) 
		{
			dpiHorizontal = dpiVertical;
		}
		else 
		{
			dpiHorizontal = 72;
			dpiVertical = 72;
		}
		
		const float pixelSizeX = static_cast<float>(ptSize * dpiHorizontal) / 72.0f;
		const float pixelSizeY = static_cast<float>(ptSize * dpiVertical) / 72.0f;
		const float scaleX = pixelSizeX / static_cast<float>(emSize);
		const float scaleY = pixelSizeY / static_cast<float>(emSize);
		
		return gml::Vec2<float>(scaleX, scaleY);
	}

	FontLoader::FontLoader(gl::TextureCreator textureCreator) noexcept
		: textureCreator(std::move(textureCreator)) {}

	Font FontLoader::loadFont(const std::string& filename, const int ptSize, const int dpi) const
	{
		FontFile fontFile;

		auto file = sys::FileLoader::openFile(filename);

		if (!file || !file->isOpen()) 
		{
			throw sys::FileException(sys::FileException::FILE_NOT_FOUND, "", filename);
		}
		
		file->seek(0, sys::IFile::SeekOffset::BEG);
		file->read((char*)&fontFile.fileHeader, sizeof(fontFile.fileHeader));
		file->read((char*)&fontFile.fontInfo, sizeof(fontFile.fontInfo));
		file->read((char*)&fontFile.sdfInfo, sizeof(fontFile.sdfInfo));
		file->read((char*)&fontFile.maxGlyph, sizeof(fontFile.maxGlyph));

		fontFile.glyphs.resize(fontFile.fontInfo.nChars);
		file->read((char*)&fontFile.glyphs[0], sizeof(Glyph) * fontFile.glyphs.size());

		fontFile.sdf.resize(fontFile.sdfInfo.width * fontFile.sdfInfo.height * fontFile.sdfInfo.channels);
		file->read((char*)&fontFile.sdf[0], sizeof(uint8_t) * fontFile.sdf.size());

		gml::Vec2<float> scale = calcScale(ptSize, fontFile.fontInfo.emSize, dpi);

		Font font;
		font.metrics.emSize = fontFile.fontInfo.emSize;
		font.metrics.ptSize = ptSize;

		font.maxGlyph.size.x = fontFile.maxGlyph.width * scale.x;
		font.maxGlyph.size.y = fontFile.maxGlyph.height * scale.y;
		font.maxGlyph.bearing.x = fontFile.maxGlyph.xBearing * scale.x;
		font.maxGlyph.bearing.y = fontFile.maxGlyph.yBearing * scale.y;
		font.maxGlyph.advance = fontFile.maxGlyph.advance * scale.x;

		for (unsigned int i = 0; i < fontFile.fontInfo.nChars; ++i)
		{
			unsigned char c = fontFile.glyphs[i].id;
			font.characters[c] = Character();
			Character & character = font.characters[c];
			
			character.glyph.size.x = fontFile.glyphs[i].width * scale.x;
			character.glyph.size.y = fontFile.glyphs[i].height * scale.y;
			character.glyph.bearing.x = fontFile.glyphs[i].xBearing * scale.x;
			character.glyph.bearing.y = fontFile.glyphs[i].yBearing * scale.y;
			character.glyph.advance = fontFile.glyphs[i].advance * scale.x;

			character.texCoords.left = fontFile.glyphs[i].texCoordLeft + 4.0f / fontFile.sdfInfo.width;
			character.texCoords.right = fontFile.glyphs[i].texCoordRight -4.0f / fontFile.sdfInfo.width;
			character.texCoords.top = fontFile.glyphs[i].texCoordTop - 4.0f / fontFile.sdfInfo.height;
			character.texCoords.bottom = fontFile.glyphs[i].texCoordBottom + 4.0f / fontFile.sdfInfo.height;
		}

		font.textureAtlas = textureCreator->createTexture2D(fontFile.sdf, fontFile.sdfInfo.width, fontFile.sdfInfo.height, gl::ImageFormat::R, 1);
		//font.textureAtlas.configTextureWrapper(gl::TextureWrapper::CLAMP_TO_EDGE, gl::TextureWrapper::CLAMP_TO_EDGE);
		//font.textureAtlas.configTextureFilter(gl::TextureFilter::LINEAR, gl::TextureFilter::LINEAR);

		return font;
	}
}
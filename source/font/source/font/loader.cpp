#include "loader.h"
#include "fileformat.h"
#include <core/platform/IFileReader.h>
#include <core/platform/IFile.h>

#include <utility>
#include <iostream>

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

	FontLoader::FontLoader(const std::shared_ptr<gl::ITextureCreator>& textureCreator) noexcept
		: textureCreator(textureCreator) {}

	std::unique_ptr<Font> FontLoader::loadFont(platform::IBinaryFile & file, const int ptSize, const int dpi) const
	{
		FontFile fontFile;
		
		file.seek(0, platform::IFile::SeekOffset::BEG);
		file.read((char*)&fontFile.fileHeader, sizeof(fontFile.fileHeader));
		file.read((char*)&fontFile.fontInfo, sizeof(fontFile.fontInfo));
		file.read((char*)&fontFile.sdfInfo, sizeof(fontFile.sdfInfo));
		file.read((char*)&fontFile.maxGlyph, sizeof(fontFile.maxGlyph));

		fontFile.glyphs.resize(fontFile.fontInfo.nChars);
		file.read((char*)&fontFile.glyphs[0], sizeof(Glyph) * fontFile.glyphs.size());

		fontFile.sdf.resize(fontFile.sdfInfo.width * fontFile.sdfInfo.height * fontFile.sdfInfo.channels);
		file.read((char*)&fontFile.sdf[0], sizeof(uint8_t) * fontFile.sdf.size());

		gml::Vec2<float> scale = calcScale(ptSize, fontFile.fontInfo.unitsPerEM, dpi);

		std::unique_ptr<Font> font = std::make_unique<Font>();
		font->metrics.ptSize = ptSize;
        font->metrics.lineHeight = fontFile.fontInfo.lineHeight * scale.y();
        font->metrics.spreadInTexCoords = {
                fontFile.fontInfo.xSpreadInTexCoords,
                fontFile.fontInfo.ySpreadInTexCoords,
        };

		font->maxGlyph.size.x() = fontFile.maxGlyph.width * scale.x();
		font->maxGlyph.size.y() = fontFile.maxGlyph.height * scale.y();
		font->maxGlyph.bearing.x() = fontFile.maxGlyph.xBearing * scale.x();
		font->maxGlyph.bearing.y() = fontFile.maxGlyph.yBearing * scale.y();
		font->maxGlyph.advance = fontFile.maxGlyph.advance * scale.x();

		for (unsigned int i = 0; i < fontFile.fontInfo.nChars; ++i)
		{
			Codepoint c = fontFile.glyphs[i].id;
			font->characters[c] = Character();
			Character & character = font->characters[c];
			
			character.glyph.size.x() = fontFile.glyphs[i].width * scale.x();
			character.glyph.size.y() = fontFile.glyphs[i].height * scale.y();
			character.glyph.bearing.x() = fontFile.glyphs[i].xBearing * scale.x();
			character.glyph.bearing.y() = fontFile.glyphs[i].yBearing * scale.y();
			character.glyph.advance = fontFile.glyphs[i].advance * scale.x();

			character.texCoords.left = fontFile.glyphs[i].texCoordLeft;
			character.texCoords.right = fontFile.glyphs[i].texCoordRight;
			character.texCoords.top = fontFile.glyphs[i].texCoordTop;
			character.texCoords.bottom = fontFile.glyphs[i].texCoordBottom;
		}

		font->textureAtlas = textureCreator->createTexture2D(fontFile.sdfInfo.width, fontFile.sdfInfo.height,
                                                             gl::ImageFormat::R, fontFile.sdf);
		font->textureAtlas->configTextureWrapper(gl::TextureWrapper::CLAMP_TO_BORDER, gl::TextureWrapper::CLAMP_TO_BORDER);
		font->textureAtlas->configTextureFilter(gl::TextureFilter::LINEAR, gl::TextureFilter::LINEAR);

		return font;
	}
}
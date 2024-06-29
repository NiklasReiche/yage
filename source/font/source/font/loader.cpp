#include "loader.h"
#include "fileformat.h"
#include <core/platform/IFileReader.h>
#include <core/platform/IFile.h>

namespace yage::font
{
    FontFileLoader::FontFileLoader(const std::shared_ptr<gl::ITextureCreator>& textureCreator,
                                   const std::shared_ptr<platform::IFileReader>& fileReader) noexcept
		: textureCreator(textureCreator), fileReader(fileReader) {}

    Font FontFileLoader::load_resource(const std::string& uri)
    {
        auto file = fileReader->openBinaryFile(uri, platform::IFile::AccessMode::READ);
        FontFile fontFile;

        file->seek(0, platform::IFile::SeekOffset::BEG);
        file->read((char*) &fontFile.fileHeader, sizeof(fontFile.fileHeader));
        file->read((char*) &fontFile.fontInfo, sizeof(fontFile.fontInfo));
        file->read((char*) &fontFile.sdfInfo, sizeof(fontFile.sdfInfo));
        file->read((char*) &fontFile.maxGlyph, sizeof(fontFile.maxGlyph));

        fontFile.glyphs.resize(fontFile.fontInfo.nChars);
        file->read((char*) &fontFile.glyphs[0], sizeof(Glyph) * fontFile.glyphs.size());

        fontFile.sdf.resize(fontFile.sdfInfo.width * fontFile.sdfInfo.height * fontFile.sdfInfo.channels);
        file->read((char*) &fontFile.sdf[0], sizeof(uint8_t) * fontFile.sdf.size());

        Font font;
        font.metrics.unitsPerEM = fontFile.fontInfo.unitsPerEM;
        font.metrics.lineHeight = fontFile.fontInfo.lineHeight;
        font.metrics.spreadInTexCoords = {
                fontFile.fontInfo.xSpreadInTexCoords,
                fontFile.fontInfo.ySpreadInTexCoords,
        };

        font.maxGlyph.size.x() = fontFile.maxGlyph.width;
        font.maxGlyph.size.y() = fontFile.maxGlyph.height;
        font.maxGlyph.bearing.x() = fontFile.maxGlyph.xBearing;
        font.maxGlyph.bearing.y() = fontFile.maxGlyph.yBearing;
        font.maxGlyph.advance = fontFile.maxGlyph.advance;

        for (unsigned int i = 0; i < fontFile.fontInfo.nChars; ++i)
        {
            Codepoint c = fontFile.glyphs[i].id;
            font.characters[c] = Character();
            Character& character = font.characters[c];

            character.glyph.size.x() = fontFile.glyphs[i].width;
            character.glyph.size.y() = fontFile.glyphs[i].height;
            character.glyph.bearing.x() = fontFile.glyphs[i].xBearing;
            character.glyph.bearing.y() = fontFile.glyphs[i].yBearing;
            character.glyph.advance = fontFile.glyphs[i].advance;

            character.texCoords.left = fontFile.glyphs[i].texCoordLeft;
            character.texCoords.right = fontFile.glyphs[i].texCoordRight;
            character.texCoords.top = fontFile.glyphs[i].texCoordTop;
            character.texCoords.bottom = fontFile.glyphs[i].texCoordBottom;
        }

        font.textureAtlas = textureCreator->createTexture2D(fontFile.sdfInfo.width, fontFile.sdfInfo.height,
                                                             gl::ImageFormat::R, fontFile.sdf);
        font.textureAtlas->configTextureWrapper(gl::TextureWrapper::CLAMP_TO_BORDER,
                                                 gl::TextureWrapper::CLAMP_TO_BORDER);
        font.textureAtlas->configTextureFilter(gl::TextureFilter::LINEAR, gl::TextureFilter::LINEAR);

        return font;
    }
}
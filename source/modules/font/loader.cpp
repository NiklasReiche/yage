#include "loader.h"

namespace font
{
	gml::Vector2D<float> FontLoader::calcScale(int ptSize, int EM_size, int dpiHori, int dpiVert)
	{
		if (dpiVert == 0 && dpiHori != 0) {
			dpiVert = dpiHori;
		}
		else if (dpiHori == 0 && dpiVert != 0) {
			dpiHori = dpiVert;
		}
		else {
			dpiHori = 72;
			dpiVert = 72;
		}
		float pixel_size_x = ptSize * dpiHori / 72.0f;
		float pixel_size_y = ptSize * dpiVert / 72.0f;
		float scale_x = pixel_size_x / EM_size;
		float scale_y = pixel_size_y / EM_size;
		return gml::Vector2D<float>(scale_x, scale_y);
	}

	FontLoader::FontLoader(sys::PlatformHandle* platform, gl::GraphicsContext* glContext)
		: platform(platform), glContext(glContext) {}

	Font FontLoader::loadFont(std::string filename, int ptsize, int dpi)
	{
		Fontfile fontfile;

		sys::File file = platform->open(filename);
		if (!file.is_open()) {
			throw FileException(FileError::PATH_VIOLATION, filename);
		}

		file.seek(0, sys::SeekOffset::BEG);
		file.read((char*)&fontfile.fileheader, sizeof(fontfile.fileheader));
		file.read((char*)&fontfile.fontinfo, sizeof(fontfile.fontinfo));
		file.read((char*)&fontfile.sdfinfo, sizeof(fontfile.sdfinfo));
		file.read((char*)&fontfile.maxglyph, sizeof(fontfile.maxglyph));

		fontfile.glyphs.resize(fontfile.fontinfo.nChars);
		file.read((char*)&fontfile.glyphs[0], sizeof(GLYPH) * fontfile.glyphs.size());

		fontfile.sdf.resize(fontfile.sdfinfo.width * fontfile.sdfinfo.height * fontfile.sdfinfo.channels);
		file.read((char*)&fontfile.sdf[0], sizeof(uint8_t) * fontfile.sdf.size());

		gml::Vec2<float> scale = calcScale(ptsize, fontfile.fontinfo.EM_size, dpi);

		Font font;
		font.metrics.EM_size = fontfile.fontinfo.EM_size;
		font.metrics.PT_size = ptsize;

		font.maxGlyph.size.x = fontfile.maxglyph.width * scale.x;
		font.maxGlyph.size.y = fontfile.maxglyph.height * scale.y;
		font.maxGlyph.bearing.x = fontfile.maxglyph.x_bearing * scale.x;
		font.maxGlyph.bearing.y = fontfile.maxglyph.y_bearing * scale.y;
		font.maxGlyph.advance = fontfile.maxglyph.advance * scale.x;

		for (unsigned int i = 0; i < fontfile.fontinfo.nChars; ++i)
		{
			unsigned char c = fontfile.glyphs[i].id;
			font.characters[c] = Character();
			Character & character = font.characters[c];
			
			character.glyph.size.x = fontfile.glyphs[i].width * scale.x;
			character.glyph.size.y = fontfile.glyphs[i].height * scale.y;
			character.glyph.bearing.x = fontfile.glyphs[i].x_bearing * scale.x;
			character.glyph.bearing.y = fontfile.glyphs[i].y_bearing * scale.y;
			character.glyph.advance = fontfile.glyphs[i].advance * scale.x;

			character.texCoords.left = fontfile.glyphs[i].texCoord_left + 4.0f / fontfile.sdfinfo.width;
			character.texCoords.right = fontfile.glyphs[i].texCoord_right -4.0f / fontfile.sdfinfo.width;
			character.texCoords.top = fontfile.glyphs[i].texCoord_top - 4.0f / fontfile.sdfinfo.height;
			character.texCoords.bottom = fontfile.glyphs[i].texCoord_bottom + 4.0f / fontfile.sdfinfo.height;
		}

		font.textureAtlas = glContext->create2DTexture(&fontfile.sdf[0], fontfile.sdfinfo.width, fontfile.sdfinfo.height, gl::ImageFormat::R, 1);
		font.textureAtlas.configTextureWrapper(gl::TextureWrapper::CLAMP_TO_EDGE, gl::TextureWrapper::CLAMP_TO_EDGE);
		font.textureAtlas.configTextureFilter(gl::TextureFilter::LINEAR, gl::TextureFilter::LINEAR);

		return font;
	}
}
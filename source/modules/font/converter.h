#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <fstream>

#include <ft2build.h>
#include FT_FREETYPE_H 

#include <graphics\Graphics.h>
#include <image\img.h>

#include "fileformat.h"
#include "font.h"
#include "ftMath.h"


namespace font
{
	typedef std::map<unsigned char, Character> charactermap;

	enum class FT_GLYPH_LOAD_FLAG : FT_Int32
	{
		BITMAP = FT_LOAD_RENDER,
		MONO = FT_LOAD_RENDER | FT_LOAD_TARGET_MONO,
		UNSCALED = FT_LOAD_RENDER | FT_LOAD_NO_SCALE
	};
	enum class ENCODING
	{
		ASCII,
		UTF8,
		UTF16
	};

	struct ConvertMetrics
	{
		unsigned char c_min;
		unsigned char c_max;

		int EM_size;
		GlyphMetrics maxGlyph;
		std::map<unsigned char, Character> characters;

		img::Image glyphImage;
		img::Image sdfImage;
	};

	class FT_Loader
	{
	private:
		FT_Library ft;
		std::vector<FT_Face> faces;
	public:
		FT_Face loadFace(std::string filepath);
		FT_GlyphSlot loadGlyph(FT_Face face, unsigned char c, FT_GLYPH_LOAD_FLAG load_flag);

		~FT_Loader();
		int initialize();
		void free(FT_Library ft);
		void free(FT_Face face);
	};

	class FontConverter
	{
	private:
		FT_Loader ft_loader;
		ConvertMetrics convertMetrics;
		gl::GraphicsContext* glContext;
	
		void loadGlyphMetrics(FT_Face face, unsigned char c, GlyphMetrics & metrics);
		void loadTextureMetrics(FT_Face face, unsigned char c, TexMetrics & metrics, gml::Vector2D<int> offset, int padding);

		gml::Vector2D<int> calcTextureSize(FT_Face face, int padding);
		void clampTo4(gml::Vector2D<int> & size);

		void generateCharacters(charactermap & characters);

		void generateGlyphMetrics(FT_Face face, charactermap & characters);
		void generateTexMetrics(FT_Face face, charactermap & characters, int padding);
		void generateMaxGlyph(const charactermap & characters, GlyphMetrics & maxGlyph);

		// SDF related
		void generateTextureAtlas(FT_Face face, img::Image & image, int padding);
		void uploadGlyphBitmaps(FT_Face face, gl::Texture & texture, int padding);
		void generateSdfTexMetrics(FT_Face face, std::map<unsigned char, TexMetrics> & metrics, int padding);
		void generateSDF(FT_Face face, img::Image & input, img::Image & output, int loadSize, int loadPadding, int resizeFactor);

		// file writing
		void writeFontfile(std::string filename);

	public:
		FontConverter(gl::GraphicsContext* glContext);

		void convertFont(std::string filename_TTF, std::string filename_FONT, ENCODING encoding = ENCODING::ASCII);
	};
}
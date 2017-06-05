#pragma once

#include <vector>
#include <cstdint>


struct FILEHEADER
{
	uint32_t identifier;
	uint32_t fsize;
	uint32_t glyphOffset;
	uint32_t imgOffset;
};

struct FONTINFO
{
	uint32_t encoding;
	uint32_t EM_size;
	uint32_t nChars;
};

struct SDFINFO
{
	uint32_t width;
	uint32_t height;
	uint32_t channels;
};

struct MAXGLYPH
{
	uint32_t width;
	uint32_t height;
	int32_t x_bearing;
	int32_t y_bearing;
	uint32_t advance;
};

struct GLYPH
{
	uint32_t id;
	uint32_t width;
	uint32_t height;
	int32_t x_bearing;
	int32_t y_bearing;
	uint32_t advance;
	float texCoord_left;
	float texCoord_right;
	float texCoord_top;
	float texCoord_bottom; 
};

struct Fontfile
{
	FILEHEADER fileheader;
	FONTINFO fontinfo;
	SDFINFO sdfinfo;

	MAXGLYPH maxglyph;
	std::vector<GLYPH> glyphs;
	std::vector<uint8_t> sdf;
};
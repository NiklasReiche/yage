#pragma once

#include <vector>
#include <cstdint>

namespace font
{
	struct FileHeader
	{
		uint32_t identifier;
        uint32_t version;
		uint32_t fSize;
		uint32_t glyphOffset;
		uint32_t imgOffset;
	};

	struct FontInfo
	{
		uint32_t unitsPerEM;
        uint32_t lineHeight;
		uint32_t nChars;
        float xSpreadInTexCoords;
        float ySpreadInTexCoords;
	};

    // TODO: add information about padding, spread, input and target resolutions
	struct SdfInfo
	{
		uint32_t width;
		uint32_t height;
		uint32_t channels;
	};

	struct MaxGlyph
	{
		uint32_t width;
		uint32_t height;
		int32_t xBearing;
		int32_t yBearing;
		uint32_t advance;
	};

	struct Glyph
	{
		uint32_t id;
		uint32_t width;
		uint32_t height;
		int32_t xBearing;
		int32_t yBearing;
		uint32_t advance;
		float texCoordLeft;
		float texCoordRight;
		float texCoordTop;
		float texCoordBottom;
	};

	struct FontFile
	{
		FileHeader fileHeader;
		FontInfo fontInfo;
		SdfInfo sdfInfo;

		MaxGlyph maxGlyph;
		std::vector<Glyph> glyphs;
		std::vector<uint8_t> sdf;
	};
}
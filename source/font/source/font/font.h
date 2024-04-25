#pragma once

#include <string>
#include <map>

#include <gml/gml.h>
#include <core/gl/Texture2D.h>
#include <memory>

// TODO: add info about spread so that we can inflate text quads appropriately for tex coords
namespace font
{
    /**
     * Encodes positioning and size values for a single glyph.
     */
	struct GlyphMetrics
	{
		gml::Vec2<float> size;
		gml::Vec2<float> bearing;
		float advance = 0;
	};

    /**
     * Encodes texture coordinates from a texture atlas for a single glyph (range: [0,1]x[0,1]).
     */
	struct TexMetrics
	{
		float top = 0;
		float bottom = 0;
		float left = 0;
		float right = 0;
	};

	struct FontMetrics
	{
		int ptSize = 0;
		int emSize = 0;
        float lineHeight = 0;
		unsigned char cMin = 0;
		unsigned char cMax = 0;
        gml::Vec2f spreadInTexCoords;
	};

	struct Character
	{
		GlyphMetrics glyph;
		TexMetrics texCoords;
	};

	struct Font
	{
		std::string name;
		FontMetrics metrics;
		GlyphMetrics maxGlyph;
		//gml::Vec2f dimensions;
		std::unique_ptr<gl::ITexture2D> textureAtlas;
		std::map<unsigned char, Character> characters;
	};
}
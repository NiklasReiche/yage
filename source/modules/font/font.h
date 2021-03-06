#pragma once

#include <string>
#include <map>

#include <math/gml.h>
#include <graphics/Texture2D.h>

namespace font
{
	struct GlyphMetrics
	{
		gml::Vec2<float> size;
		gml::Vec2<float> bearing;
		float advance = 0;
	};

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
		unsigned char cMin = 0;
		unsigned char cMax = 0;
	};

	struct Character
	{
		GlyphMetrics glyph;
		TexMetrics texCoords;
	};

	struct Font
	{
		std::string name = "";
		FontMetrics metrics;
		GlyphMetrics maxGlyph;
		//gml::Vec2f dimensions;
		gl::Texture2D textureAtlas;
		std::map<unsigned char, Character> characters;
	};
}
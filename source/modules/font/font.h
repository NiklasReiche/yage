#pragma once

#include <string>
#include <map>

#include <math\MVector.h>
#include <graphics\Graphics.h>

namespace font
{
	/*****************************************
	**			Font related types			**
	*****************************************/

	struct GlyphMetrics
	{
		gml::Vector2D<float> size;
		gml::Vector2D<float> bearing;
		float advance = 0;
	};

	struct TexMetrics
	{
		float top;
		float bottom;
		float left;
		float right;
	};

	struct FontMetrics
	{
		int PT_size = 0;
		int EM_size = 0;
		unsigned char c_min = 0;
		unsigned char c_max = 0;
	};

	struct Character
	{
		GlyphMetrics glyph;
		TexMetrics texCoords;
	};

	struct Font
	{
		std::string name = "arial";
		FontMetrics metrics;
		GlyphMetrics maxGlyph;
		gl::Texture textureAtlas;
		std::map<unsigned char, Character> characters;
	};
}
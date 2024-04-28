#pragma once

#include <string>
#include <map>

#include <gml/gml.h>
#include <core/gl/Texture2D.h>
#include <memory>

namespace font
{
    /**
     * Represents Unicode code points.
     */
    typedef uint32_t Codepoint;

    /**
     * Encodes positioning and size values for a single glyph. Values are given in pixel coordinates (i.e. scaled to
     * the pt size and dpi given when the font was loaded).
     */
    struct GlyphMetrics
    {
        /**
         * Size of the glyph's bounding box in pixel coordinates.
         */
		gml::Vec2<float> size;

        /**
         * x: Vertical distance from the cursor position to the left edge of the glyph's bounding box in pixel coordinates.
         * y: Horizontal distance from the baseline to the top edge of the glyph's bounding box in pixel coordinates.
         */
        gml::Vec2<float> bearing;

        /**
         * Distance that the cursor should increment after drawing this glyph in pixel coordinates.
         */
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
        /**
         * Pt size at which the font was loaded. Use this for calculating scaling factors when rendering text at
         * different pt sizes.
         */
		int ptSize = 0; // TODO: maybe instead store unitsPerEM and offer a method to calculate scale for a given pt size and dpi

        /**
         * Vertical space between two baselines. Use this to calculate newlines.
         */
        float lineHeight = 0;

        /**
         * Spread of the sdf in texture coordinates.
         */
        gml::Vec2f spreadInTexCoords;
	};

	struct Character
	{
		GlyphMetrics glyph;
		TexMetrics texCoords;
	};

	struct Font
	{
        /**
         * General metrics about the font.
         */
		FontMetrics metrics;

        /**
         * Encodes the maximum value across all glyphs for each metric separately.
         */
        GlyphMetrics maxGlyph;

        /**
         * Metrics for the individual characters.
         */
        std::map<Codepoint, Character> characters; // TODO: unordered_map?

        /**
         * Texture atlas containing the glyph sdf bitmaps.
         */
        std::unique_ptr<gl::ITexture2D> textureAtlas;
	};
}
#pragma once

#include <string>
#include <unordered_map>

#include <math/math.h>
#include <core/gl/Texture2D.h>
#include <memory>

namespace yage::font
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
		math::Vec2f size;

        /**
         * x: Vertical distance from the cursor position to the left edge of the glyph's bounding box in pixel coordinates.
         * y: Horizontal distance from the baseline to the top edge of the glyph's bounding box in pixel coordinates.
         */
        math::Vec2f bearing;

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
         * Font units per EM square.
         */
        int unitsPerEM = 0;

        /**
         * Vertical space between two baselines. Use this to calculate newlines.
         */
        float lineHeight = 0;

        /**
         * Spread of the sdf in texture coordinates.
         */
        math::Vec2f spreadInTexCoords;
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
        std::unordered_map<Codepoint, Character> characters;

        /**
         * Texture atlas containing the glyph sdf bitmaps.
         */
        std::unique_ptr<gl::ITexture2D> textureAtlas;

        /**
         * Returns the scaling factor to derive pixel coordinates for glyph metrics for a given font size and dpi
         * resolution.
         * @param ptSize Font size at which to draw text.
         * @param dpi DPI resolution of the target canvas.
         * @return Scaling factor to convert glyph metrics into pixel coordinates.
         */
        [[nodiscard]]
        math::Vec2f scaling(float ptSize, math::Vec2i dpi) const
        {
            // 1 pt = 1/72 inch
            const math::Vec2f pixelsPerEM = static_cast<math::Vec2f>(dpi) * ptSize / 72.f;
            return pixelsPerEM / static_cast<float>(metrics.unitsPerEM);
        }
	};
}
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

#include <core/gl/graphics.h>

#include "font/fileformat.h"
#include "font/font.h"
#include "ftMath.h"
#include "img.h"


namespace font
{
    enum class FT_GLYPH_LOAD_FLAG : FT_Int32
    {
        MONOCHROME_BITMAP = FT_LOAD_RENDER | FT_LOAD_TARGET_MONO,
        UNSCALED = FT_LOAD_NO_SCALE
    };

    class FT_Loader
    {
    public:
        ~FT_Loader();

        /**
         * Must be called before loading faces or glyphs.
         */
        void initialize();

        FT_Face loadFace(const std::string &filepath);

        FT_GlyphSlot loadGlyph(const FT_Face &face, unsigned char c, FT_GLYPH_LOAD_FLAG load_flag);

    private:
        /**
         * Handle for the freetype library
         */
        FT_Library ft;

        /**
         * Tracks all faces loaded through freetype, so that we can free them in the destructor.
         */
        std::vector<FT_Face> faces;
    };

    class FontConverter
    {
    public:
        explicit FontConverter(const std::shared_ptr<gl::IContext> &glContext);

        /**
         * Converts a font from a .ttf file to an sdf font and writes the result to a .font file (internal format).
         *
         * @param filenameInput Path of the .ttf file containing the font to convert.
         * @param filenameOutput Path to which the .font file should be written.
         * @param loadResolution Size in pixels with which to read glyphs from the .ttf file. The larger this is, the higher quality the resulting sdf will be.
         * @param spread Size in pixels (in the sdf resolution space) that the sdf extends from the glyph edges.
         * @param padding Padding in pixels (in the sdf resolution space) added around sdf glyphs in the texture atlas.
         * @param sdfResolution Size in pixels at which to down-sample the sdf glyphs. The smaller this is the smaller the resulting texture atlas will be.
         */
        void convert(const std::string &filenameInput, const std::string &filenameOutput, int loadResolution = 512,
                     int spread = 4, int padding = 2, int sdfResolution = 64);

    private:
        std::shared_ptr<gl::IContext> glContext;

        static GlyphMetrics getGlyphMetricsAndUpdateMax(FT_Loader &ft, const FT_Face &face, unsigned char c, GlyphMetrics &maxGlyph);

        static img::Image getBitmap(FT_Loader &ft, FT_Face const &face, unsigned char c);

        static img::Image generateSdf(const img::Image &bitmap, int spread);

        /**
         * Reads a color value from an non-padded input image using coordinates from a padded image space.
         * @param bitmap input image without padding
         * @param y vertical lookup coordinate in padded space
         * @param x horizontal lookup coordinate in padded space
         * @param padding padding in texture coordinate fractions around the input bitmap
         * @return value taken from the input image or zero if the coordinates fall within the padded space
         */
        static unsigned char getPaddedValue(const img::Image &bitmap, int y, int x, int padding);

        static TexMetrics
        getRelativeTextureMetrics(const img::Image &characterBitmap, gml::Vec2i offset, gml::Vec2i atlasSize);

        img::Image downscale(const img::Image &image, gml::Vec2i targetResolution);

        static void writeFontFile(const std::string &filename, int unitsPerEM, unsigned char c_min, unsigned char c_max,
                                  const img::Image &atlas,
                                  GlyphMetrics maxGlyph, const std::map<unsigned char, Character> &characters,
                                  gml::Vec2f spreadInTexCoords);

        static unsigned char rightGetBit(unsigned char c, unsigned int n);

        /**
         * Unpacks a tightly packed bitmap image to a byte-map.
         * @param data Data where 1 byte encodes 8 values.
         * @param image unpacked output image
         * @param rows input rows
         * @param width input row width
         * @param pitch
         */
        static std::vector<unsigned char>
        unpackBitmap(unsigned char *data, unsigned int rows, unsigned int width, unsigned int pitch);
    };
}
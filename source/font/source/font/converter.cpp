#include <chrono>
#include "converter.h"
#include "image/img.h"
#include "core/platform/desktop/FileReader.h"

namespace font
{
    /*****************************************
    **			FreeType Loader 			**
    *****************************************/

    FT_Loader::~FT_Loader()
    {
        // free all loaded faces
        for (unsigned int i = 0; i < faces.size(); ++i)
        {
            FT_Done_Face(faces[i]);
            faces.erase(faces.begin() + i);
        }
        // free library handle
        FT_Done_FreeType(ft);
    }

    void FT_Loader::initialize()
    {
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }
    }

    FT_Face FT_Loader::loadFace(const std::string &filepath)
    {
        FT_Face face;
        FT_Error error = FT_New_Face(ft, filepath.c_str(), 0, &face);
        if (error)
        {
            std::cout << "ERROR::FREETYPE: Failed to load face \n" << error << std::endl; // TODO: throw exception
        } else
        {
            faces.push_back(face);
        }
        return face;
    }

    FT_GlyphSlot
    FT_Loader::loadGlyph(const FT_Face &face, unsigned char c, // NOLINT(*-convert-member-functions-to-static)
                         FT_GLYPH_LOAD_FLAG load_flag)
    {
        FT_Error error = FT_Load_Char(face, c, (FT_Int32) load_flag);
        if (error)
        {
            std::cout << "ERROR::FREETYPE: Failed to load glyph \n" << error << std::endl; // TODO: throw exception
        }
        return face->glyph;
    }


    /*****************************************
    **			SDF Generator	 			**
    *****************************************/

    FontConverter::FontConverter(const std::shared_ptr<gl::IContext> &glContext)
            : glContext(glContext)
    {}

    void FontConverter::convert(const std::string &filenameInput, const std::string &filenameOutput,
                                const int loadResolution, const int spread, const int padding, const int sdfResolution)
    {
        // TODO: allow unicode (give a set of codepoints instead of a char range)
        const unsigned char c_min = 32;
        const unsigned char c_max = 127;

        const int scaleFactor = loadResolution / sdfResolution;

        FT_Loader ft;
        ft.initialize();
        FT_Face face = ft.loadFace(filenameInput);
        FT_Set_Pixel_Sizes(face, 0, loadResolution);

        // we place the glyphs into the atlas in a roughly square grid
        const int nCharacters = c_max - c_min;
        const int nColumns = std::ceil(std::sqrt(nCharacters));

        std::map<unsigned char, img::Image> sdfMap;
        gml::Vec2i atlasSize;
        gml::Vec2i rowSize;
        int column = 0;
        // traverse characters and append them to the atlas grid from left to right and top to bottom (i.e. row-major)
        for (unsigned char c = c_min; c < c_max; c++)
        {
            auto bitmap = getBitmap(ft, face, c);
            // the spread is given in target resolution pixels, so we need to upscale it here
            auto sdf = generateSdf(bitmap, spread * scaleFactor);
            sdfMap[c] = downscale(sdf, gml::Vec2i(sdf.getWidth() / scaleFactor,
                                                  sdf.getHeight() / scaleFactor));

            rowSize.x() += sdfMap[c].getWidth() + 2 * padding;
            rowSize.y() = std::max(rowSize.y(), sdfMap[c].getHeight() + 2 * padding);

            column++;
            // switch to new row
            if (column > nColumns)
            {
                atlasSize.x() = std::max(atlasSize.x(), rowSize.x());
                atlasSize.y() += rowSize.y();
                rowSize = gml::Vec2i();
                column = 0;
            }
            std::cout << c << std::endl;
        }
        // add the last row if it hasn't been added inside the loop
        if (column > 0)
        {
            atlasSize.x() = std::max(atlasSize.x(), rowSize.x());
            atlasSize.y() += rowSize.y();
        }

        // unbind the frame buffers used for downscaling the sdf
        glContext->getRenderer()->setDefaultRenderTarget();

        // we start with a black image and consecutively upload glyphs
        std::vector<unsigned char> emptyImage(atlasSize.x() * atlasSize.y(), 0);
        std::unique_ptr<gl::ITexture2D> textureAtlas = glContext->getTextureCreator()->createTexture2D(
                atlasSize.x(), atlasSize.y(), gl::ImageFormat::R, emptyImage);

        std::map<unsigned char, Character> characters;
        GlyphMetrics maxGlyph;
        gml::Vec2i offset;
        rowSize = gml::Vec2i();
        column = 0;
        // traverse characters again and use same packing algorithm as above to get correct texture coordinates
        for (unsigned char c = c_min; c < c_max; c++)
        {
            img::Image &sdf = sdfMap[c];

            // compute glyph metrics
            characters[c] = Character{
                .glyph = getGlyphMetricsAndUpdateMax(ft, face, c, maxGlyph),
                .texCoords = getRelativeTextureMetrics(sdf, offset + gml::Vec2i(padding), atlasSize)
            };

            // add sdf to texture atlas
            auto subArea = utils::Area(offset.x() + padding, offset.y() + padding,
                                       sdf.getWidth(), sdf.getHeight());
            textureAtlas->setSubImage(subArea, {sdf.data(), sdf.getSize()});

            // advance offsets
            offset += gml::Vec2i(sdf.getWidth() + 2 * padding, 0);
            rowSize.y() = std::max(rowSize.y(), sdf.getHeight() + 2 * padding);

            column++;
            // switch to new row
            if (column > nColumns)
            {
                offset = gml::Vec2i(0, offset.y() + rowSize.y());
                rowSize = gml::Vec2i();
                column = 0;
            }
        }

        const gml::Vec2f spreadInTexCoords = {
                (float) spread / (float) atlasSize.x(),
                (float) spread / (float) atlasSize.y()
        };

        auto atlasImageData = textureAtlas->getImage();
        img::Image atlasImage(textureAtlas->getWidth(), textureAtlas->getHeight(), 1, atlasImageData);
        writeFontFile(filenameOutput, face->units_per_EM, c_min, c_max, atlasImage, maxGlyph, characters, spreadInTexCoords, face);
#if 1 // TODO: make this an output option
        {
            platform::desktop::FileReader fileReader;
            auto file = fileReader.openBinaryFile(R"(C:\Users\Niklas\Downloads\sdf.bmp)",
                                                  platform::IFile::AccessMode::WRITE);
            img::writeToFile(*file, atlasImage);
        }
#endif
    }

    img::Image FontConverter::getBitmap(FT_Loader &ft, const FT_Face &face, unsigned char c)
    {
        FT_GlyphSlot glyph = ft.loadGlyph(face, c, FT_GLYPH_LOAD_FLAG::MONOCHROME_BITMAP);
        FT_Bitmap &bitmap = glyph->bitmap;

        std::vector<unsigned char> imageData = unpackBitmap(bitmap.buffer, bitmap.rows, bitmap.width, bitmap.pitch);
        return {(int) bitmap.width, (int) bitmap.rows, 1, imageData};
    }

    GlyphMetrics FontConverter::getGlyphMetricsAndUpdateMax(FT_Loader &ft, const FT_Face &face, unsigned char c,
                                                            GlyphMetrics &maxGlyph)
    {
        FT_GlyphSlot glyph = ft.loadGlyph(face, c, FT_GLYPH_LOAD_FLAG::UNSCALED);

        GlyphMetrics metrics{
                .size = gml::Vec2f((float) glyph->metrics.width,
                                   (float) glyph->metrics.height),
                .bearing = gml::Vec2f((float) glyph->metrics.horiBearingX,
                                      (float) glyph->metrics.horiBearingY),
                .advance = (float) glyph->metrics.horiAdvance
        };

        maxGlyph.size.x() = std::max(maxGlyph.size.x(), metrics.size.x());
        maxGlyph.size.y() = std::max(maxGlyph.size.y(), metrics.size.y());
        maxGlyph.bearing.x() = std::max(maxGlyph.bearing.x(), metrics.bearing.x());
        maxGlyph.bearing.y() = std::max(maxGlyph.bearing.y(), metrics.bearing.y());
        maxGlyph.advance = std::max(maxGlyph.advance, metrics.advance);

        return metrics;
    }

    TexMetrics
    FontConverter::getRelativeTextureMetrics(const img::Image &characterBitmap, gml::Vec2i offset, gml::Vec2i atlasSize)
    {
        TexMetrics metrics;
        metrics.left = (float) offset.x() / (float) atlasSize.x();
        metrics.top = (float) offset.y() / (float) atlasSize.y();
        // range must be inclusive of width and height, since dividing by size samples the left edge of the pixel
        metrics.right = (float) (offset.x() + characterBitmap.getWidth()) / (float) atlasSize.x();
        metrics.bottom = (float) (offset.y() + characterBitmap.getHeight()) / (float) atlasSize.y();
        return metrics;
    }

    img::Image FontConverter::generateSdf(const img::Image &bitmap, const int spread)
    {
        // Padding around the input bitmap must be at least equal to spread, so we don't cut off edges and corners.
        // We add a small padding around the sdf to reduce interference when sampling the atlas for text rendering.
        const int bitmapPadding = spread;
        const gml::Vec2i sdfSize(bitmap.getWidth() + 2 * bitmapPadding, bitmap.getHeight() + 2 * bitmapPadding);
        img::Image sdf(sdfSize.x(), sdfSize.y(), 1);

        // squared distance is sufficient for comparisons
        const double maxDistanceSqr = spread * spread;
        const double maxDistance = spread;
        // iterate through pixels
        gml::Vec2i p;
        for (p.y() = 0; p.y() < sdfSize.y(); p.y()++)
        {
            for (p.x() = 0; p.x() < sdfSize.x(); p.x()++)
            {
                const unsigned char value = getPaddedValue(bitmap, p.y(), p.x(), bitmapPadding);

                // The search area must at least encompass a circle around the pixel with a radius of 'spread'.
                // We use a rectangle around this circle for simplicity. Pixels within this search area but outside
                // the circle (i.e. the corners) will have an actual distance larger than the max distance (i.e. the
                // radius), and so will be assigned the max distance in the end.
                const gml::Vec2i searchAreaStart(std::max(0, p.x() - spread),
                                                 std::max(0, p.y() - spread));
                const gml::Vec2i searchAreaEnd(std::min(sdfSize.x(), p.x() + spread),
                                               std::min(sdfSize.y(), p.y() + spread));

                // iterate through search area around pixel, tracking the minimal distance to an opposite color pixel
                auto minDistanceSqr = maxDistanceSqr;
                gml::Vec2i neighbour;
                for (neighbour.y() = searchAreaStart.y(); neighbour.y() < searchAreaEnd.y(); neighbour.y()++)
                {
                    for (neighbour.x() = searchAreaStart.x(); neighbour.x() < searchAreaEnd.x(); neighbour.x()++)
                    {
                        // find pixel with the opposite value
                        const unsigned char neighbourValue = getPaddedValue(bitmap, neighbour.y(), neighbour.x(),
                                                                            bitmapPadding);
                        if (neighbourValue == value)
                        {
                            continue;
                        }

                        // squared distance is sufficient for comparisons
                        const double distanceSqr = gml::sqrLength(neighbour - p);
                        if (distanceSqr < minDistanceSqr)
                        {
                            minDistanceSqr = distanceSqr;
                        }
                    }
                }

                // use actual distance for the normalization instead of squared for a smoother output range
                double minDistance = std::sqrt(minDistanceSqr);
                // outer pixels get negative distances, inner pixels get positive distances, so that the edge is at zero
                if (value == 0)
                {
                    minDistance *= -1;
                }
                sdf(p.y(), p.x(), 0) = (unsigned char) gml::normalize(minDistance,
                                                                      -maxDistance, maxDistance,
                                                                      0, 255);
            }
        }

        return sdf;
    }

    unsigned char FontConverter::getPaddedValue(const img::Image &bitmap, const int y, const int x, const int padding)
    {
        if (y < padding || x < padding || y > bitmap.getHeight() - 1 + padding || x > bitmap.getWidth() - 1 + padding)
            return 0; // we pad with black pixels
        else
            return bitmap(y - padding, x - padding, 0);
    }

    img::Image FontConverter::downscale(const img::Image &image, gml::Vec2i targetResolution)
    {
        // load image into texture
        std::unique_ptr<gl::ITexture2D> texture = glContext->getTextureCreator()
                ->createTexture2D(image.getWidth(), image.getHeight(), gl::ImageFormat::R,
                                  {image.data(), image.data() + image.getSize()});
        texture->configTextureWrapper(gl::TextureWrapper::CLAMP_TO_BORDER, gl::TextureWrapper::CLAMP_TO_BORDER);
        texture->configTextureFilter(gl::TextureFilter::LINEAR, gl::TextureFilter::LINEAR);

        // render texture to frame buffer of the target resolution and let the GPU handle the down-sampling
        auto frame = glContext->getFrameCreator()
                ->createFrame(targetResolution.x(), targetResolution.y(), gl::ImageFormat::R);
        auto renderer = glContext->getRenderer();
        renderer->setRenderTarget(*frame);
        renderer->setViewport(0, 0, targetResolution.x(), targetResolution.y());
        renderer->draw(*texture); // draws the texture on a quad that fills the viewport

        // download image data from GPU
        auto scaledTexture = frame->getTexture();
        auto scaledImageData = scaledTexture->getImage();
        return {
                scaledTexture->getWidth(),
                scaledTexture->getHeight(),
                1,
                scaledImageData
        };
    }

    void
    FontConverter::writeFontFile(const std::string &filename, int unitsPerEM, unsigned char c_min, unsigned char c_max,
                                 const img::Image &atlas, GlyphMetrics maxGlyph,
                                 const std::map<unsigned char, Character> &characters,
                                 const gml::Vec2f spreadInTexCoords, const FT_Face & face)
    {
        FontFile fontFile;

        fontFile.fontInfo.encoding = 1;
        fontFile.fontInfo.unitsPerEM = unitsPerEM;
        fontFile.fontInfo.lineHeight = face->height;
        fontFile.fontInfo.nChars = c_max - c_min;
        fontFile.fontInfo.xSpreadInTexCoords = spreadInTexCoords.x();
        fontFile.fontInfo.ySpreadInTexCoords = spreadInTexCoords.y();

        fontFile.sdfInfo.width = (uint32_t) atlas.getWidth();
        fontFile.sdfInfo.height = (uint32_t) atlas.getHeight();
        fontFile.sdfInfo.channels = (uint32_t) atlas.getDepth();

        fontFile.maxGlyph.width = (uint32_t) maxGlyph.size.x();
        fontFile.maxGlyph.height = (uint32_t) maxGlyph.size.y();
        fontFile.maxGlyph.xBearing = (int32_t) maxGlyph.bearing.x();
        fontFile.maxGlyph.yBearing = (int32_t) maxGlyph.bearing.y();
        fontFile.maxGlyph.advance = (uint32_t) maxGlyph.advance;

        for (unsigned char c = c_min; c < c_max; ++c)
        {
            // TODO: whats up with the data types?
            Glyph glyph{
                    .id = c,
                    .width = (uint32_t) characters.at(c).glyph.size.x(),
                    .height = (uint32_t) characters.at(c).glyph.size.y(),
                    .xBearing = (int32_t) characters.at(c).glyph.bearing.x(),
                    .yBearing = (int32_t) characters.at(c).glyph.bearing.y(),
                    .advance = (uint32_t) characters.at(c).glyph.advance,

                    .texCoordLeft = characters.at(c).texCoords.left,
                    .texCoordRight = characters.at(c).texCoords.right,
                    .texCoordTop = characters.at(c).texCoords.top,
                    .texCoordBottom = characters.at(c).texCoords.bottom
            };
            fontFile.glyphs.push_back(glyph);
        }

        fontFile.sdf = std::vector<unsigned char>(atlas.data(), atlas.data() + atlas.getSize());

        fontFile.fileHeader.identifier = 0x464f4e54;
        fontFile.fileHeader.fSize = sizeof(fontFile);
        fontFile.fileHeader.glyphOffset = sizeof(FileHeader) + sizeof(FontInfo) + sizeof(SdfInfo) + sizeof(MaxGlyph);
        fontFile.fileHeader.imgOffset = sizeof(FileHeader) + sizeof(FontInfo) + sizeof(SdfInfo) + sizeof(MaxGlyph) +
                                        sizeof(Glyph) * fontFile.glyphs.size();


        std::ofstream file(filename, std::ios::binary);
        file.seekp(0, std::ios::beg);
        file.write((char *) &fontFile.fileHeader, sizeof(fontFile.fileHeader));
        file.write((char *) &fontFile.fontInfo, sizeof(fontFile.fontInfo));
        file.write((char *) &fontFile.sdfInfo, sizeof(fontFile.sdfInfo));
        file.write((char *) &fontFile.maxGlyph, sizeof(fontFile.maxGlyph));
        file.write((char *) &fontFile.glyphs[0], sizeof(Glyph) * fontFile.glyphs.size());
        file.write((char *) &fontFile.sdf[0], sizeof(uint8_t) * fontFile.sdf.size());
    }

    unsigned char FontConverter::rightGetBit(unsigned char c, unsigned int n)
    {
        return ((c << n) & 128) >> 7;
    }

    std::vector<unsigned char>
    FontConverter::unpackBitmap(unsigned char *data, unsigned int rows, unsigned int width, unsigned int pitch)
    {
        std::vector<unsigned char> image;
        unsigned int bit_pointer = 0;

        for (unsigned int i = 0; i < rows; i++)
        {
            unsigned char *row = &data[i * pitch];

            while (bit_pointer < width)
            {
                unsigned char v;

                if (rightGetBit(row[bit_pointer / 8], bit_pointer % 8) == 1)
                {
                    v = 255;
                } else if (rightGetBit(row[bit_pointer / 8], bit_pointer % 8) == 0)
                {
                    v = 0;
                }
                image.push_back(v);

                bit_pointer++;
            }
            bit_pointer = 0;
        }
        return image;
    }
}
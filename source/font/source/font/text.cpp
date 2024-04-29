#include "text.h"
#include <core/gl/color.h>
#include <array>
#include <algorithm>
#include <codecvt>

namespace font
{
    Text::Text(const std::shared_ptr<gl::IDrawableCreator>& drawableCreator,
               const std::u32string& text,
               const res::Resource<Font>& font,
               const TextParameters& params)
            : drawableCreator(drawableCreator), fontResource(font), text(text), fontSize(params.ptSize),
              dpi(params.dpi), color(gl::toVec4(params.color))
    {
        if (!text.empty())
        {
            std::vector<float> vertices;
            std::vector<unsigned int> indices;
            constructVertices(vertices, indices);
            drawable = drawableCreator->createDrawable(vertices, indices, std::vector<unsigned int>{2, 2, 4},
                                                       gl::VertexFormat::BATCHED);
        }
    }

    gml::Vec2<float> Text::getSize() const
    {
        return size;
    }

    gml::Vec4<float> Text::getColor() const
    {
        return color;
    }

    void Text::constructVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices)
    {
        size = gml::Vec2f(0.0f);
        std::vector<float> coords;
        constructVertexCoords(coords, indices);
        vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));

        std::vector<float> texCoords;
        constructVertexTexCoords(texCoords);
        vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));

        std::vector<float> colors;
        constructVertexColors(colors);
        vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
    }

    void Text::constructVertexCoords(std::vector<float>& vertices, std::vector<unsigned int>& indices)
    {
        auto& font = fontResource.get();
        const auto scale = font.getScaling(fontSize, dpi);

        float xPos = 0;
        float yPos = 0;

        std::vector<float> coords;
        unsigned int index = 0;

        // Iterate through all characters
        for (char32_t c: text)
        {
            if (c == '\n')
            {
                xPos = 0;
                yPos += font.metrics.lineHeight * scale.y();
                size.y() += font.metrics.lineHeight * scale.y();
                continue;
            }

            if (!font.characters.contains(c))
            {
                c = '?';
            }

            const Character& ch = font.characters.at(c);

            const float left = xPos + ch.glyph.bearing.x() * scale.x();
            const float right = left + ch.glyph.size.x() * scale.x();
            const float top = yPos + (font.maxGlyph.bearing.y() * scale.y() - ch.glyph.bearing.y() * scale.y());
            const float bottom = top + ch.glyph.size.y() * scale.y();

            // Generate glyph quad for each character
            std::array<float, 8> localCoords = {
                    left, top,
                    left, bottom,
                    right, bottom,
                    right, top,
            };

            // Append to global coords
            coords.insert(std::end(coords), std::begin(localCoords), std::end(localCoords));
            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index + 2);
            indices.push_back(index + 3);
            indices.push_back(index);
            index += 4;

            // Advance cursors for next glyph
            xPos += ch.glyph.advance * scale.x();

            // Update text geometry
            size.x() = std::max(size.x(), xPos);
            size.y() = std::max(size.y(), yPos + bottom);
        }

        // Append to global vertices
        vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));
    }

    void Text::constructVertexTexCoords(std::vector<float>& vertices)
    {
        auto& font = fontResource.get();
        std::vector<float> texCoords;

        for (char32_t c: text)
        {
            if (c == '\n')
                continue;

            if (!font.characters.contains(c))
            {
                c = '?';
            }

            const Character& ch = font.characters.at(c);

            const float spreadX = font.metrics.spreadInTexCoords.x() * (1 - spreadFactor);
            const float spreadY = font.metrics.spreadInTexCoords.y() * (1 - spreadFactor);

            const float texLeft = ch.texCoords.left + spreadX;
            const float texRight = ch.texCoords.right - spreadX;
            const float texBottom = ch.texCoords.bottom - spreadY;
            const float texTop = ch.texCoords.top + spreadY;

            // Generate glyph quad for each character
            std::array<float, 8> localTexCoords = {
                    texLeft, texTop,
                    texLeft, texBottom,
                    texRight, texBottom,
                    texRight, texTop,
            };

            // Append to global tex coords
            texCoords.insert(std::end(texCoords), std::begin(localTexCoords), std::end(localTexCoords));
        }

        // Append to global vertices
        vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));
    }

    void Text::constructVertexColors(std::vector<float>& vertices)
    {
        std::vector<float> colors;

        for (char32_t c: text)
        {
            if (c == '\n')
                continue;

            // add values even for unknown characters, since we substitute them with '?'

            // Generate glyph quad for each character
            std::array<float, 16> localColors = {
                    color.x(), color.y(), color.z(), color.w(),
                    color.x(), color.y(), color.z(), color.w(),
                    color.x(), color.y(), color.z(), color.w(),
                    color.x(), color.y(), color.z(), color.w(),
            };

            // Append to global colors
            colors.insert(std::end(colors), std::begin(localColors), std::end(localColors));
        }

        // Append to global vertices
        vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
    }

    const gl::ITexture2D& Text::getTexture()
    {
        return *(fontResource.get().textureAtlas);
    }

    const gl::IDrawable& Text::getDrawable() const
    {
        return *drawable;
    }

    std::u32string Text::getString() const
    {
        return text;
    }

    gml::Vec2f Text::getMaxDimensions()
    {
        auto& font = fontResource.get();
        const auto scale = font.getScaling(fontSize, dpi);

        gml::Vec2f dimensions;
        dimensions.x() = font.maxGlyph.size.x() * scale.x();
        dimensions.y() = (font.maxGlyph.size.y() + (font.maxGlyph.size.y() - font.maxGlyph.bearing.y())) * scale.y();

        return dimensions;
    }

    gml::Vec2f Text::getOffset(const unsigned int index)
    {
        if (index > text.length())
        {
            return gml::Vec2f(-1.0f);
        }

        auto& font = fontResource.get();
        const auto scale = font.getScaling(fontSize, dpi);

        gml::Vec2f offset(0.0f);
        for (unsigned int i = 0; i < index; ++i)
        {
            const Character& ch = font.characters.at(text[i]);
            offset.x() += ch.glyph.advance * scale.x();
        }

        // TODO: ?
        //const Character & ch = font.characters.at(text[index]);
        //offset.y = font.maxGlyph.bearing.y * scale - ch.glyph.bearing.y * scale;

        return offset;
    }
}

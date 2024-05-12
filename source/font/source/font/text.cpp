#include "text.h"
#include <core/gl/color.h>
#include <array>
#include <algorithm>
#include <codecvt>

namespace font
{
    Text::Text(const std::shared_ptr<gl::IDrawableCreator>& drawable_creator,
               const std::u32string& text,
               const res::Resource<Font>& font,
               const TextParameters& params)
            : m_drawable_creator(drawable_creator), m_font_resource(font), m_text(text), m_spread_factor(params.spread_factor),
              m_font_size(params.pt_size), m_dpi(params.dpi), m_color(gl::toVec4(params.color)), m_offset(params.offset)
    {
        if (!text.empty()) {
            std::vector<float> vertices = construct_vertices();
            std::vector<unsigned int> indices = construct_indices();
            m_drawable = drawable_creator->createDrawable(vertices, indices, std::vector<unsigned int>{3, 2, 4},
                                                          gl::VertexFormat::BATCHED);
        }
    }

    std::vector<float> Text::construct_vertices()
    {
        m_dimensions = gml::Vec2f(0.0f);
        std::vector<float> vertices;

        m_data_offset_coords = 0;
        std::vector<float> coords = construct_vertex_coords();
        vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));

        m_data_offset_tex_coords = coords.size();
        std::vector<float> texCoords = construct_vertex_tex_coords();
        vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));

        m_data_offset_colors = m_data_offset_tex_coords + texCoords.size();
        std::vector<float> colors = construct_vertex_colors();
        vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));

        return vertices;
    }

    std::vector<float> Text::construct_vertex_coords()
    {
        auto& font = m_font_resource.get();
        const auto scale = font.scaling(m_font_size, m_dpi);

        float xPos = m_offset.x();
        float yPos = m_offset.y();
        float zPos = m_offset.z();

        std::vector<float> coords;

        // Iterate through all characters
        for (char32_t c: m_text) {
            if (c == '\n') {
                xPos = m_offset.x();
                yPos += font.metrics.lineHeight * scale.y();
                m_dimensions.y() += font.metrics.lineHeight * scale.y();
                continue;
            }

            if (!font.characters.contains(c)) {
                c = '?';
            }

            const Character& ch = font.characters.at(c);

            const float left = xPos + ch.glyph.bearing.x() * scale.x();
            const float right = left + ch.glyph.size.x() * scale.x();
            const float top = yPos + (font.maxGlyph.bearing.y() * scale.y() - ch.glyph.bearing.y() * scale.y());
            const float bottom = top + ch.glyph.size.y() * scale.y();

            // Generate glyph quad for each character
            std::array<float, 12> localCoords = {
                    left, top, zPos,
                    left, bottom, zPos,
                    right, bottom, zPos,
                    right, top, zPos
            };

            // Append to global coords
            coords.insert(std::end(coords), std::begin(localCoords), std::end(localCoords));

            // Advance cursors for next glyph
            xPos += ch.glyph.advance * scale.x();

            // Update text geometry
            m_dimensions.x() = std::max(m_dimensions.x(), xPos - m_offset.x());
            m_dimensions.y() = std::max(m_dimensions.y(), yPos - m_offset.y() + bottom);
        }

        // Append to global vertices
        return coords;
    }

    std::vector<float> Text::construct_vertex_tex_coords()
    {
        auto& font = m_font_resource.get();
        std::vector<float> tex_coords;

        for (char32_t c: m_text) {
            if (c == '\n')
                continue;

            if (!font.characters.contains(c)) {
                c = '?';
            }

            const Character& ch = font.characters.at(c);

            const float spread_x = font.metrics.spreadInTexCoords.x() * (1 - m_spread_factor);
            const float spread_y = font.metrics.spreadInTexCoords.y() * (1 - m_spread_factor);

            const float tex_left = ch.texCoords.left + spread_x;
            const float tex_right = ch.texCoords.right - spread_x;
            const float tex_bottom = ch.texCoords.bottom - spread_y;
            const float tex_top = ch.texCoords.top + spread_y;

            // Generate glyph quad for each character
            std::array<float, 8> local_tex_coords = {
                    tex_left, tex_top,
                    tex_left, tex_bottom,
                    tex_right, tex_bottom,
                    tex_right, tex_top,
            };

            // Append to global tex coords
            tex_coords.insert(std::end(tex_coords), std::begin(local_tex_coords), std::end(local_tex_coords));
        }

        // Append to global vertices
        return tex_coords;
    }

    std::vector<float> Text::construct_vertex_colors()
    {
        std::vector<float> colors;

        for (char32_t c: m_text) {
            if (c == '\n')
                continue;

            // add values even for unknown characters, since we substitute them with some glyph

            // Generate glyph quad for each character
            std::array<float, 16> localColors = {
                    m_color.x(), m_color.y(), m_color.z(), m_color.w(),
                    m_color.x(), m_color.y(), m_color.z(), m_color.w(),
                    m_color.x(), m_color.y(), m_color.z(), m_color.w(),
                    m_color.x(), m_color.y(), m_color.z(), m_color.w(),
            };

            // Append to global colors
            colors.insert(std::end(colors), std::begin(localColors), std::end(localColors));
        }

        // Append to global vertices
        return colors;
    }

    std::vector<unsigned int> Text::construct_indices()
    {
        std::vector<unsigned int> indices;
        unsigned int index = 0;
        for (char32_t c: m_text) {
            if (c == '\n')
                continue;

            // add values even for unknown characters, since we substitute them with some glyph

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index + 2);
            indices.push_back(index + 3);
            indices.push_back(index);
            index += 4;
        }
        return indices;
    }

    const gl::ITexture2D& Text::texture()
    {
        return *(m_font_resource.get().textureAtlas);
    }

    const gl::IDrawable& Text::drawable() const
    {
        return *m_drawable;
    }

    std::u32string Text::text() const
    {
        return m_text;
    }

    gml::Vec2f Text::max_font_dimensions()
    {
        auto& font = m_font_resource.get();
        const auto scale = font.scaling(m_font_size, m_dpi);

        gml::Vec2f dimensions;
        dimensions.x() = font.maxGlyph.size.x() * scale.x();
        dimensions.y() = (font.maxGlyph.size.y() + (font.maxGlyph.size.y() - font.maxGlyph.bearing.y())) * scale.y();

        return dimensions;
    }

    gml::Vec3f Text::offset(unsigned int i)
    {
        if (i > m_text.length()) {
            throw std::invalid_argument("index out of bounds");
        }

        auto& font = m_font_resource.get();
        const auto scale = font.scaling(m_font_size, m_dpi);

        gml::Vec3f _offset = m_offset;
        for (unsigned int j = 0; j < i; ++i) {
            const Character& ch = font.characters.at(m_text[j]);
            _offset.x() += ch.glyph.advance * scale.x();
        }

        return _offset;
    }

    gml::Vec2<float> Text::dimensions() const
    {
        return m_dimensions;
    }

    gml::Vec4<float> Text::color() const
    {
        return m_color;
    }

    void Text::update_offset(gml::Vec3f offset)
    {
        m_offset = offset;
        std::vector<float> vertices = construct_vertex_coords();
        m_drawable->setSubData(m_data_offset_coords, vertices);
    }

    void Text::update_offset(gml::Vec2f offset)
    {
        m_offset.x() = offset.x();
        m_offset.y() = offset.y();
        update_offset(m_offset);
    }

    void Text::update_color(gl::Color_t color)
    {
        m_color = gl::toVec4(color);
        std::vector<float> vertices = construct_vertex_colors();
        m_drawable->setSubData(m_data_offset_colors, vertices);
    }

    void Text::update_size(float pt_size, gml::Vec2i dpi)
    {
        m_font_size = pt_size;
        m_dpi = dpi;
        std::vector<float> vertices = construct_vertex_coords();
        m_drawable->setSubData(m_data_offset_coords, vertices);
    }
}

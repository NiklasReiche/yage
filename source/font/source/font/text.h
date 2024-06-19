#pragma once

#include <string>
#include <vector>

#include <core/gl/DrawableCreator.h>
#include <math/vector.h>
#include "resource/Resource.h"
#include "font.h"
#include "core/gl/Texture2D.h"
#include "core/gl/color.h"

namespace yage::font
{
    /**
     * Optional constructor parameters for text objects.
     */
    struct TextParameters
    {
        /**
         * Text color.
         */
        gl::Color_t color = gl::Color::BLACK;

        /**
         * Font size in pt. Used to compute the size of the text geometry.
         */
        float pt_size = 14;

        /**
         * Resolution (dots per inch) of the target canvas. Used to compute the size of the text geometry.
         */
        math::Vec2i dpi = math::Vec2i(72);

        /**
         * Offset in model space for the text geometry. By default, text geometry starts at the origin of the model
         * space, translated into the negative z-plane for easier screen projection.
         */
        math::Vec3f offset = math::Vec3f(0, 0, -1);

        /**
         * How much of the sdf spread is used. Increase this if you need thicker spread for shader effects
         * (e.g. borders, glow, etc.).
         */
        float spread_factor = 0.05;
    };

    /**
     * Represent geometry for a string of UTF-32 encoded text that can be rendered.
     * The geometry is constructed in 3D model space. Dynamic transformations should be done in the shader.
     */
     // TODO: Can we do inserting/appending without just destroying and recreating the whole vertex data? Maybe allocate GPU memory like a vector
    class Text
    {
    public:
        Text() = delete;

        Text(const std::shared_ptr<gl::IDrawableCreator>& drawable_creator,
             const std::u32string& text,
             const res::Resource<Font>& font,
             const TextParameters& params = TextParameters{});

        /**
         * @return Size of the text geometry in model space.
         */
        [[nodiscard]]
        math::Vec2f dimensions() const;

        /**
         * @return Color of this text.
         */
        [[nodiscard]]
        math::Vec4f color() const;

        /**
         * @return Texture atlas of the underlying font needed when drawing this text.
         */
        [[nodiscard]]
        const gl::ITexture2D& texture();

        /**
         * @return Drawable containing the text geometry.
         */
        [[nodiscard]]
        const gl::IDrawable& drawable() const;

        /**
         * @return Underlying string of this text.
         */
        [[nodiscard]]
        std::u32string text() const;

        /**
         * @return Maximal dimensions of the underlying font and scale of this text
         */
        [[nodiscard]]
        math::Vec2f max_font_dimensions();

        /**
         * Returns the model space offset of the i-th character glyph geometry. Note that this includes any initial
         * offset of the whole text. If the index is out of bounds, returns the offset where the next character would
         * appear.
         * @param i Index of the character in the text string
         * @return Model space coordinates of the glyph
         */
        [[nodiscard]]
        math::Vec3f offset(std::size_t i);

        /**
         * Returns the model space offset of the i-th character glyph geometry from the start of the text. The first
         * character therefore has an offset of zero. If the index is out of bounds, returns the offset where the next
         * character would appear.
         * @param i Index of the character in the text string
         * @return Model space coordinates of the glyph
         */
        [[nodiscard]]
        math::Vec3f relative_offset(std::size_t i);

        /**
         * @return Font size in pt of this text.
         */
        [[nodiscard]] float font_size() const;

        /**
         * Reconstructs the text geometry at an updated 3D offset.
         * @param offset New offset to use.
         */
        void update_offset(math::Vec3f offset);

        /**
         * Reconstructs the text geometry at an updated 2D offset. The z-coordinate remains unchanged.
         * @param offset New offset to use.
         */
        void update_offset(math::Vec2f offset);

        /**
         * Reconstructs the text geometry with an updated color.
         * @param color New color to use.
         */
        void update_color(gl::Color_t color);

        /**
         * Reconstructs the text geometry at an updated font size.
         * @param pt_size New font size in pt to use.
         * @param dpi New dpi resolution to use.
         */
        void update_size(float pt_size, math::Vec2i dpi);

    private:
        std::shared_ptr<gl::IDrawableCreator> m_drawable_creator;
        std::unique_ptr<gl::IDrawable> m_drawable;
        res::Resource<Font> m_font_resource;

        std::u32string m_text;
        float m_spread_factor;
        float m_font_size;
        math::Vec2i m_dpi;
        math::Vec4f m_color;
        math::Vec3f m_offset;
        math::Vec2f m_dimensions;

        unsigned int m_data_offset_coords = 0;
        unsigned int m_data_offset_tex_coords = 0;
        unsigned int m_data_offset_colors = 0;

        [[nodiscard]]
        std::vector<unsigned int> construct_indices();

        [[nodiscard]]
        std::vector<float> construct_vertices();

        [[nodiscard]]
        std::vector<float> construct_vertex_coords();

        [[nodiscard]]
        std::vector<float> construct_vertex_tex_coords();

        [[nodiscard]]
        std::vector<float> construct_vertex_colors();

        [[nodiscard]]
        math::Vec3f char_offset(std::size_t i, math::Vec3f initial);
    };
}

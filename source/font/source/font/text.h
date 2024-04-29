#pragma once

#include <string>
#include <vector>

#include <core/gl/DrawableCreator.h>
#include <gml/vector.h>
#include "resource/Resource.h"
#include "font.h"
#include "core/gl/Texture2D.h"
#include "core/gl/color.h"

namespace font
{
    /**
     * Optional constructor parameters for text objects.
     */
    struct TextParameters
    {
        /**
         * Text color.
         */
        uint32_t color = gl::Color::BLACK;

        /**
         * Font size in pt. Used to compute the size of the text geometry.
         */
        float ptSize = 14;

        /**
         * Resolution (dots per inch) of the target canvas. Used to compute the size of the text geometry.
         */
        gml::Vec2i dpi = gml::Vec2i(72);

        /**
         * Offset in model space for the text geometry. By default, text geometry starts at the origin of the model
         * space.
         */
        gml::Vec3f offset = gml::Vec3f(0, 0, 0);

        /**
         * How much of the sdf spread is used. Increase this if you need thicker spread for shader effects
         * (e.g. borders, glow, etc.).
         */
        float spreadFactor = 0.05;
    };

    /**
     * Represent geometry for a string of UTF-32 encoded text that can be rendered.
     * The geometry is constructed in 2D model space, i.e. scaling, translation, etc.. should be done in the shader.
     */
    class Text
    {
    public:
        Text() = delete;

        Text(const std::shared_ptr<gl::IDrawableCreator>& drawableCreator,
             const std::u32string& text,
             const res::Resource<Font>& font,
             const TextParameters& params = TextParameters{});

        [[nodiscard]]
        gml::Vec2<float> getSize() const;

        [[nodiscard]]
        gml::Vec4<float> getColor() const;

        [[nodiscard]]
        const gl::ITexture2D& getTexture();

        [[nodiscard]]
        const gl::IDrawable& getDrawable() const;

        [[nodiscard]]
        std::u32string getString() const;

        [[nodiscard]]
        gml::Vec2f getMaxDimensions(); // TODO: this might be better suited as a method of the font

        gml::Vec2f getOffset(unsigned int index); //TODO: is this needed?

    private:
        std::shared_ptr<gl::IDrawableCreator> drawableCreator;
        std::unique_ptr<gl::IDrawable> drawable;
        res::Resource<Font> fontResource;

        std::u32string text;
        float spreadFactor = 0.05;
        float fontSize;
        gml::Vec2i dpi;
        gml::Vec4<float> color;

        gml::Vec2<float> size;

        void constructVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices);

        void constructVertexCoords(std::vector<float>& vertices, std::vector<unsigned int>& indices);

        void constructVertexTexCoords(std::vector<float>& vertices);

        void constructVertexColors(std::vector<float>& vertices);
    };
}

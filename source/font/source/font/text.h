#pragma once

#include <string>
#include <vector>

#include <core/gl/DrawableCreator.h>
#include <gml/vector.h>

#include "font.h"
#include "core/gl/Texture2D.h"

namespace font
{
    // TODO: maybe use a builder pattern to append text with different parameters (e.g. font, size or color)

	class Text
	{
	public:
		Text() = delete;
		Text(const std::shared_ptr<gl::IDrawableCreator>& drawableCreator,
		     const std::string & text, 
		     const std::shared_ptr<Font> & font,
		     unsigned int color = 0x000000FFu, 
		     int size = 14); // TODO: param struct could make sense here

		[[nodiscard]]
		gml::Vec2<float> getSize() const;
		[[nodiscard]]
		gml::Vec4<float> getColor() const;

		[[nodiscard]]
		const gl::ITexture2D& getTexture() const;
		[[nodiscard]]
        const gl::IDrawable& getDrawable() const;
		[[nodiscard]]
		std::string getString() const;
		[[nodiscard]]
		gml::Vec2f getMaxDimensions() const; // TODO: this might be better suited as a method of the font
		gml::Vec2f getOffset(unsigned int index); //TODO: is this needed?

	private:
        const float SPREAD_KEEP_PERCENT = 0.05; // TODO: make this configurable

		std::shared_ptr<gl::IDrawableCreator> drawableCreator;
        std::unique_ptr<gl::IDrawable> drawable;
		std::shared_ptr<Font> font;

		gml::Vec2<float> size;
		gml::Vec4<float> color;

		std::string text;
		int fontSize = 16;

		void constructVertices(std::vector<float> & vertices, std::vector<unsigned int> &indices);

		void constructVertexCoords(std::vector<float> & vertices, std::vector<unsigned int> &indices);
		void constructVertexTexCoords(std::vector<float> & vertices);
		void constructVertexColors(std::vector<float> & vertices);
	};
}

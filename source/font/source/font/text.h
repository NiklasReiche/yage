#pragma once

#include <string>
#include <vector>

#include <core/gl/DrawableCreator.h>
#include <gml/vector.h>

#include "font.h"
#include "core/gl/Texture2D.h"

namespace font
{
	/**
	 * @brief 
	 * 
	 */
	class Text
	{
	public:
		Text() = delete;
		Text(const std::shared_ptr<gl::IDrawableCreator>& drawableCreator,
		     const std::string & text, 
		     const std::shared_ptr<Font> & font,
		     const gml::Vec3<float>& position = gml::Vec3<float>(0.0f),
		     unsigned int color = 0x000000FFu, 
		     int size = 14);

		[[nodiscard]]
		gml::Vec2<float> getSize() const;
		[[nodiscard]]
		gml::Vec3<float> getPosition() const;
		[[nodiscard]]
		gml::Vec4<float> getColor() const;

		[[nodiscard]]
		const gl::ITexture2D& getTexture() const;
		[[nodiscard]]
        const gl::IDrawable& getDrawable() const;
		[[nodiscard]]
		std::string getString() const;
		[[nodiscard]]
		gml::Vec2f getMaxDimensions() const;
		gml::Vec2f getOffset(unsigned int index);

	private:
		std::shared_ptr<gl::IDrawableCreator> drawableCreator;
        std::unique_ptr<gl::IDrawable> drawable;
		std::shared_ptr<Font> font;

		gml::Vec3<float> position;
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

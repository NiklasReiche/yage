#pragma once

#include <string>
#include <vector>

#include <graphics/DrawableCreator.h>

#include "font.h"

namespace font
{
	/**
	 * @brief 
	 * 
	 */
	class Text
	{
	public:
		Text() = default;
		Text(const gl::DrawableCreator& drawableCreator, 
		     const std::string & text, 
		     const Font & font,
		     const gml::Vec2<float>& position = gml::Vec2<float>(0.0f), 
		     unsigned int color = 0x000000FFu, 
		     int size = 14);

		[[nodiscard]]
		gml::Vec2<float> getSize() const;
		[[nodiscard]]
		gml::Vec2<float> getPosition() const;
		[[nodiscard]]
		gml::Vec4<float> getColor() const;

		void setText(const std::string& text, const Font & font);
		void appendText(const std::string& text, const Font & font);
		void insertText(const std::string& text, const Font & font, int offset = 0);

		void setColor(unsigned int color);

		void setPosition(const gml::Vec2<float>& position);

		[[nodiscard]]
		gl::Texture2D getTexture() const;
		[[nodiscard]]
		gl::Drawable getDrawable() const;
		[[nodiscard]]
		std::string getString() const;
		[[nodiscard]]
		gml::Vec2f getMaxDimensions() const;
		gml::Vec2f getOffset(int index);

	private:
		gl::DrawableCreator drawableCreator;
		gl::Drawable drawable;
		gl::Texture2D textureAtlas;
		Font font;

		gml::Vec2<float> position;
		gml::Vec2<float> size;
		gml::Vec4<float> color;

		std::string text;
		int fontSize = 16;

		int VERTEX_SIZE_COORDS = 12;
		int VERTEX_SIZE_TEXCOORDS = 12;
		int VERTEX_SIZE_COLORS = 24;
		int vertexOffsetCoords = 0;
		int vertexOffsetTexCoords = 0;
		int vertexOffsetColors = 0;

		void constructVertices(std::vector<float> & vertices, const Font & font);

		void constructVertexCoords(std::vector<float> & vertices, const Font & font);
		void constructVertexTexCoords(std::vector<float> & vertices, const Font & font);
		void constructVertexColors(std::vector<float> & vertices);
	};
}

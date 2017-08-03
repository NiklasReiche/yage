#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "font.h"

namespace font
{
	/*****************************************
	**			Drawable Text				**
	*****************************************/

	class Text : public gl::Drawable
	{
	private:
		gl::Texture textureAtlas;
		font::Font font;

		gml::Vec2<float> position;
		gml::Vec2<float> size;
		gml::Vec4<float> color;

		std::string text;
		int fontSize = 16;

		int VERTEX_SIZE_COORDS = 12;
		int VERTEX_SIZE_TEXCOORDS = 12;
		int VERTEX_SIZE_COLORS = 24;
		int vertexOffsetCoords;
		int vertexOffsetTexCoords;
		int vertexOffsetColors;

		void constructVertices(std::vector<GLfloat> & vertices, const Font & font);

		void constructVertexCoords(std::vector<GLfloat> & vertices, const Font & font);
		void constructVertexTexCoords(std::vector<GLfloat> & vertices, const Font & font);
		void constructVertexColors(std::vector<GLfloat> & vertices);

	public:
		Text(){}
		Text(gl::GraphicsContext* glContext, std::string text, const Font & font, gml::Vec2<float> position = gml::Vec2<float>(0.0f), unsigned int color = 0x000000FFu, int size = 14);

		gml::Vec2<float> getSize() { return size; }
		gml::Vec2<float> getPosition() { return position; }
		gml::Vec4<float> getColor() { return color; }

		void setText(std::string text, const Font & font);
		void appendText(std::string text, const Font & font);
		void insertText(std::string text, const Font & font, int offset = 0);

		void setColor(unsigned int color);

		void setPosition(gml::Vec2<float> position);

		gl::Texture getTexture();
		std::string getString();
		gml::Vec2f getMaxDimensions();
		gml::Vec2f getOffset(int index);
	};
}
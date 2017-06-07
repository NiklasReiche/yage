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

		gml::Vector2D<float> position;
		gml::Vector2D<float> size;
		gml::Vector4D<float> color;

		std::string text;
		int fontSize = 16;
		int linePitch = 5;

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

		gml::Vector2D<float> getSize() { return size; }
		gml::Vector2D<float> getPosition() { return position; }
		gml::Vector4D<float> getColor() { return color; }

		void setText(std::string text, const Font & font);
		void appendText(std::string text, const Font & font);

		void setColor(unsigned int color);

		gl::Texture getTexture();
	};
}
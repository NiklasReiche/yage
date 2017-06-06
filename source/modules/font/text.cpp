#include "text.h"

namespace font
{
	/*****************************************
	**			Drawable Text				**
	*****************************************/

	Text::Text(gl::GraphicsContext glContext, std::string text, const Font & font, gml::Vec2<float> position, unsigned int color, int size)
		: text(text), position(position), color(gl::toVec4(color)), fontSize(size)
	{
		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices, font);
		this->textureAtlas = font.textureAtlas;
		glContext.createDrawable(*this, vertices, std::vector<int> { 2, 2, 4 }, gl::DrawMode::DRAW_DYNAMIC, gl::VertexFormat::BATCHED);
	}

	void Text::constructVertices(std::vector<gl::Gfloat>& vertices, const Font & font)
	{
		std::vector<gl::Gfloat> coords;
		constructVertexCoords(coords, font);
		vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));

		std::vector<gl::Gfloat> texCoords;
		constructVertexTexCoords(texCoords, font);
		vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));
		
		std::vector<gl::Gfloat> colors;
		constructVertexColors(colors);
		vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
	}

	void Text::constructVertexCoords(std::vector<gl::Gfloat>& vertices, const Font & font)
	{
		float scale = fontSize / (float)font.metrics.PT_size;

		float globalWidth = (float)font.textureAtlas.getWidth();
		float globalHeight = (float)font.textureAtlas.getHeight();

		float xpos = position.x;
		float ypos = position.y;

		std::vector<gl::Gfloat> coords;

		// Iterate through all characters
		for (unsigned int i = 0; i < text.length(); ++i)
		{
			const Character & ch = font.characters.at(text[i]);

			gl::Gfloat left = xpos + ch.glyph.bearing.x * scale;
			gl::Gfloat right = left + ch.glyph.size.x * scale;
			gl::Gfloat top = ypos + (font.maxGlyph.bearing.y * scale - ch.glyph.bearing.y * scale);
			gl::Gfloat bottom = top + ch.glyph.size.y * scale;

			// Generate glyph quad for each character
			std::array<gl::Gfloat, 12> localCoords = {
				left, top,
				left, bottom,
				right,	bottom,

				right,	bottom,
				right,	top,
				left,	top
			};

			// Append to global coords
			coords.insert(std::end(coords), std::begin(localCoords), std::end(localCoords));

			// Advance cursors for next glyph
			xpos += (ch.glyph.advance) * scale;

			// Update text geometry
			size.x = std::max(size.x, xpos - position.x);
			size.y = std::max(size.y, ypos - position.y + bottom - position.y);
		}

		// Append to global vertices
		vertexOffsetCoords = 0;
		vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));
	}
	void Text::constructVertexTexCoords(std::vector<gl::Gfloat>& vertices, const Font & font)
	{
		std::vector<gl::Gfloat> texCoords;

		for (unsigned int i = 0; i < text.length(); ++i)
		{
			const Character & ch = font.characters.at(text[i]);

			gl::Gfloat texLeft = ch.texCoords.left;
			gl::Gfloat texRight = ch.texCoords.right;
			gl::Gfloat texBottom = ch.texCoords.bottom;
			gl::Gfloat texTop = ch.texCoords.top;

			// Generate glyph quad for each character
			std::array<gl::Gfloat, 12> localTexCoords = {
				texLeft, texTop,
				texLeft, texBottom,
				texRight, texBottom,

				texRight, texBottom,
				texRight, texTop,
				texLeft, texTop
			};

			// Append to global tex coords
			texCoords.insert(std::end(texCoords), std::begin(localTexCoords), std::end(localTexCoords));
		}

		// Append to global vertices
		vertexOffsetTexCoords = VERTEX_SIZE_COORDS * text.length();
		vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));
	}
	void Text::constructVertexColors(std::vector<gl::Gfloat>& vertices)
	{
		std::vector<gl::Gfloat> colors;

		for (unsigned int i = 0; i < text.length(); ++i)
		{
			// Generate glyph quad for each character
			std::array<gl::Gfloat, 24> localColors = {
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w,
				color.x, color.y, color.z, color.w
			};

			// Append to global colors
			colors.insert(std::end(colors), std::begin(localColors), std::end(localColors));
		}

		// Append to global vertices
		vertexOffsetColors = vertexOffsetTexCoords + VERTEX_SIZE_TEXCOORDS * text.length();
		vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
	}

	void Text::setText(std::string text, const Font & font)
	{
		this->text = text;

		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices, font);
		this->textureAtlas = font.textureAtlas;
		bufferSubData(0, vertices);
	}

	void Text::appendText(std::string text, const Font & font)
	{
		this->text += text;

		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices, font);
		bufferSubData(0, vertices);


	}


	void Text::setColor(unsigned int icolor)
	{
		this->color = gl::toVec4(icolor);

		std::vector<gl::Gfloat> vertices;
		constructVertexColors(vertices);
		bufferSubData(vertexOffsetColors, vertices);
	}

	gl::Texture Text::getTexture()
	{
		return textureAtlas;
	}
}
#include "text.h"
#include <graphics/color.h>
#include <array>
#include <algorithm>
#include <map>

namespace font
{
	Text::Text(const gl::DrawableCreator& drawableCreator, 
	           const std::string & text, 
	           const Font & font, 
	           const gml::Vec2<float>& position, 
	           const unsigned int color, 
	           const int size)
		: drawableCreator(drawableCreator), font(font), position(position), color(gl::toVec4(color)), text(text), fontSize(size)
	{
		this->textureAtlas = font.textureAtlas;

		if (text.length() > 0) {
			std::vector<float> vertices;
			constructVertices(vertices, font);
			drawable = drawableCreator->createDrawable(vertices, std::vector<int> { 2, 2, 4 }, gl::VertexFormat::BATCHED);
		}
	}

	gml::Vec2<float> Text::getSize() const
	{
		return size;
	}
	
	gml::Vec2<float> Text::getPosition() const
	{
		return position;
	}
	
	gml::Vec4<float> Text::getColor() const
	{
		return color;
	}

	void Text::constructVertices(std::vector<float>& vertices, const Font & font)
	{
		size = gml::Vec2f(0.0f);
		std::vector<float> coords;
		constructVertexCoords(coords, font);
		vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));

		std::vector<float> texCoords;
		constructVertexTexCoords(texCoords, font);
		vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));
		
		std::vector<float> colors;
		constructVertexColors(colors);
		vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
	}

	void Text::constructVertexCoords(std::vector<float>& vertices, const Font & font)
	{
		const float scale = static_cast<float>(fontSize) / static_cast<float>(font.metrics.ptSize);

		float xPos = position.x;
		const float yPos = position.y;

		std::vector<float> coords;

		// Iterate through all characters
		for (char i : text)
		{
			const Character & ch = font.characters.at(i);

			const float left = xPos + ch.glyph.bearing.x * scale;
			const float right = left + ch.glyph.size.x * scale;
			const float top = yPos + (font.maxGlyph.bearing.y * scale - ch.glyph.bearing.y * scale);
			const float bottom = top + ch.glyph.size.y * scale;

			// Generate glyph quad for each character
			std::array<float, 12> localCoords = {
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
			xPos += ch.glyph.advance * scale;

			// Update text geometry
			size.x = std::max(size.x, xPos - position.x);
			size.y = std::max(size.y, yPos - position.y + bottom - position.y);
		}

		// Append to global vertices
		vertexOffsetCoords = 0;
		vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));
	}
	
	void Text::constructVertexTexCoords(std::vector<float>& vertices, const Font & font)
	{
		std::vector<float> texCoords;

		for (char i : text)
		{
			const Character & ch = font.characters.at(i);

			const float texLeft = ch.texCoords.left;
			const float texRight = ch.texCoords.right;
			const float texBottom = ch.texCoords.bottom;
			const float texTop = ch.texCoords.top;

			// Generate glyph quad for each character
			std::array<float, 12> localTexCoords = {
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
	
	void Text::constructVertexColors(std::vector<float>& vertices)
	{
		std::vector<float> colors;

		for (unsigned int i = 0; i < text.length(); ++i)
		{
			// Generate glyph quad for each character
			std::array<float, 24> localColors = {
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

	void Text::setText(const std::string& text, const Font & font)
	{
		const std::string oldText = this->text;
		this->text = text;

		if (text.length() > 0) {
			std::vector<float> vertices;
			constructVertices(vertices, font);
			textureAtlas = font.textureAtlas;
			if (oldText.length() == 0) {
				drawableCreator->createDrawable(vertices, std::vector<int> { 2, 2, 4 }, gl::VertexFormat::BATCHED);
			}
			else {
				drawable->setData(vertices);
			}
		}
	}
	
	void Text::appendText(const std::string& text, const Font & font)
	{
		this->text += text;

		std::vector<float> vertices;
		constructVertices(vertices, font);
		textureAtlas = font.textureAtlas;
		drawable->setData(vertices);
	}
	
	void Text::insertText(const std::string& text, const Font & font, const int offset)
	{
		this->text.insert(offset, text);

		std::vector<float> vertices;
		constructVertices(vertices, font);
		textureAtlas = font.textureAtlas;
		drawable->setData(vertices);
	}

	void Text::setColor(const unsigned int color)
	{
		this->color = gl::toVec4(color);

		std::vector<float> vertices;
		constructVertexColors(vertices);
		drawable->setSubData(vertexOffsetColors, vertices);
	}
	
	void Text::setPosition(const gml::Vec2<float>& position)
	{
		this->position = position;
		std::vector<float> vertices;
		constructVertices(vertices, font);
		drawable->setSubData(0, vertices);
	}

	gl::Texture2D Text::getTexture() const
	{
		return textureAtlas;
	}

	gl::Drawable Text::getDrawable() const
	{
		return drawable;
	}

	std::string Text::getString() const
	{
		return text;
	}
	
	gml::Vec2f Text::getMaxDimensions() const
	{
		const float scale = static_cast<float>(fontSize) / static_cast<float>(font.metrics.ptSize);

		gml::Vec2f dimensions;
		dimensions.x = font.maxGlyph.size.x * scale;
		dimensions.y = (font.maxGlyph.size.y + (font.maxGlyph.size.y - font.maxGlyph.bearing.y)) * scale;

		return dimensions;
	}
	
	gml::Vec2f Text::getOffset(const int index)
	{
		if (index > text.length()){
			return gml::Vec2f(-1.0f);
		}
		
		gml::Vec2f offset(0.0f);
		const float scale = static_cast<float>(fontSize) / static_cast<float>(font.metrics.ptSize);

		for (unsigned int i = 0; i < index; ++i) {
			const Character & ch = font.characters.at(text[i]);
			offset.x += ch.glyph.advance * scale;
		}

		//const Character & ch = font.characters.at(text[index]);
		//offset.y = font.maxGlyph.bearing.y * scale - ch.glyph.bearing.y * scale;

		return offset;
	}
}

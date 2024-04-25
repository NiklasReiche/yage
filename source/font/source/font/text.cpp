#include "text.h"
#include <core/gl/color.h>
#include <array>
#include <algorithm>
#include <map>

namespace font
{
	Text::Text(const std::shared_ptr<gl::IDrawableCreator>& drawableCreator,
	           const std::string & text, 
	           const std::shared_ptr<Font> & font,
	           const gml::Vec3<float>& position,
	           const unsigned int color, 
	           const int size)
		: drawableCreator(drawableCreator), font(font), position(position), color(gl::toVec4(color)), text(text), fontSize(size)
	{
		if (!text.empty()) {
			std::vector<float> vertices;
            std::vector<unsigned int> indices;
			constructVertices(vertices, indices);
			drawable = drawableCreator->createDrawable(vertices, indices, std::vector<unsigned int> { 3, 2, 4 }, gl::VertexFormat::BATCHED);
		}
	}

	gml::Vec2<float> Text::getSize() const
	{
		return size;
	}
	
	gml::Vec3<float> Text::getPosition() const
	{
		return position;
	}
	
	gml::Vec4<float> Text::getColor() const
	{
		return color;
	}

	void Text::constructVertices(std::vector<float>& vertices, std::vector<unsigned int>& indices)
	{
		size = gml::Vec2f(0.0f);
		std::vector<float> coords;
		constructVertexCoords(coords, indices);
		vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));

		std::vector<float> texCoords;
		constructVertexTexCoords(texCoords);
		vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));
		
		std::vector<float> colors;
		constructVertexColors(colors);
		vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
	}

	void Text::constructVertexCoords(std::vector<float>& vertices, std::vector<unsigned int> &indices)
	{
		const float scale = static_cast<float>(fontSize) / static_cast<float>(font->metrics.ptSize);

		float xPos = position.x();
		const float yPos = position.y();
        const float zPos = position.z();

		std::vector<float> coords;
        unsigned int index = 0;

		// Iterate through all characters
		for (char i : text)
		{
			const Character & ch = font->characters.at(i);

			const float left = xPos + ch.glyph.bearing.x() * scale;
			const float right = left + ch.glyph.size.x() * scale;
			const float top = yPos + (font->maxGlyph.bearing.y() * scale - ch.glyph.bearing.y() * scale);
			const float bottom = top + ch.glyph.size.y() * scale;

			// Generate glyph quad for each character
			std::array<float, 12> localCoords = {
				left, top, zPos,
				left, bottom, zPos,
                right, bottom, zPos,
				right, top, zPos,
			};

			// Append to global coords
			coords.insert(std::end(coords), std::begin(localCoords), std::end(localCoords));
            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index + 2);
            indices.push_back(index + 3);
            indices.push_back(index);
            index += 4;

			// Advance cursors for next glyph
			xPos += ch.glyph.advance * scale;

			// Update text geometry
			size.x() = std::max(size.x(), xPos - position.x());
			size.y() = std::max(size.y(), yPos - position.y() + bottom - position.y());
		}

		// Append to global vertices
		vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));
	}
	
	void Text::constructVertexTexCoords(std::vector<float>& vertices)
	{
		std::vector<float> texCoords;

		for (char i : text)
		{
			const Character & ch = font->characters.at(i);

            const float spreadX = font->metrics.spreadInTexCoords.x() * (1 - SPREAD_KEEP_PERCENT);
            const float spreadY = font->metrics.spreadInTexCoords.y() * (1 - SPREAD_KEEP_PERCENT);

			const float texLeft = ch.texCoords.left + spreadX;
			const float texRight = ch.texCoords.right - spreadX;
			const float texBottom = ch.texCoords.bottom - spreadY;
			const float texTop = ch.texCoords.top + spreadY;

			// Generate glyph quad for each character
			std::array<float, 8> localTexCoords = {
				texLeft, texTop,
				texLeft, texBottom,
				texRight, texBottom,
				texRight, texTop,
			};

			// Append to global tex coords
			texCoords.insert(std::end(texCoords), std::begin(localTexCoords), std::end(localTexCoords));
		}

		// Append to global vertices
		vertices.insert(std::end(vertices), std::begin(texCoords), std::end(texCoords));
	}
	
	void Text::constructVertexColors(std::vector<float>& vertices)
	{
		std::vector<float> colors;

		for (unsigned int i = 0; i < text.length(); ++i)
		{
			// Generate glyph quad for each character
			std::array<float, 16> localColors = {
				color.x(), color.y(), color.z(), color.w(),
				color.x(), color.y(), color.z(), color.w(),
                color.x(), color.y(), color.z(), color.w(),
                color.x(), color.y(), color.z(), color.w(),
			};

			// Append to global colors
			colors.insert(std::end(colors), std::begin(localColors), std::end(localColors));
		}

		// Append to global vertices
		vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
	}

    const gl::ITexture2D& Text::getTexture() const
	{
		return *font->textureAtlas;
	}

    const gl::IDrawable& Text::getDrawable() const
	{
		return *drawable;
	}

	std::string Text::getString() const
	{
		return text;
	}
	
	gml::Vec2f Text::getMaxDimensions() const
	{
		const float scale = static_cast<float>(fontSize) / static_cast<float>(font->metrics.ptSize);

		gml::Vec2f dimensions;
		dimensions.x() = font->maxGlyph.size.x() * scale;
		dimensions.y() = (font->maxGlyph.size.y() + (font->maxGlyph.size.y() - font->maxGlyph.bearing.y())) * scale;

		return dimensions;
	}
	
	gml::Vec2f Text::getOffset(const unsigned int index)
	{
		if (index > text.length()){
			return gml::Vec2f(-1.0f);
		}
		
		gml::Vec2f offset(0.0f);
		const float scale = static_cast<float>(fontSize) / static_cast<float>(font->metrics.ptSize);

		for (unsigned int i = 0; i < index; ++i) {
			const Character & ch = font->characters.at(text[i]);
			offset.x() += ch.glyph.advance * scale;
		}

		//const Character & ch = font.characters.at(text[index]);
		//offset.y = font.maxGlyph.bearing.y * scale - ch.glyph.bearing.y * scale;

		return offset;
	}
}

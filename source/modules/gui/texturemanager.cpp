#include "texturemanager.h"
#include <image/imageReader.h>

namespace gui 
{
	TextureManager::TextureManager(sys::PlatformHandle* platform, gl::GraphicsContext* glContext)
		: platform(platform), glContext(glContext)
	{
		textures[0].texture = glContext->create2DTexture(nullptr, TEXTURE_SIZE, TEXTURE_SIZE, gl::ImageFormat::RGBA);
		textures.at(0).texture.bufferSubData(0, 0, 2, 2, std::vector<unsigned char>(16, 0));
		textures.at(0).cursor.x = 2;
		textures.at(0).cursor.y = 0;
		textures.at(0).y_nextOffset = 1;
	}

	gml::Vec4f TextureManager::addTexture(const img::Image & image)
	{
		gml::Vec2f& cursor = textures.at(textures.size()-1).cursor;
		int& y_plus = textures.at(textures.size()-1).y_nextOffset;
		gl::Texture& texture = textures.at(textures.size()-1).texture;

		gml::Vec4f texCoords;

		if (cursor.x + image.getWidth() > texture.getWidth()) {
			cursor.x = 0;
			cursor.y += y_plus;
			y_plus = 0;
		}

		if (cursor.y + image.getHeight() > texture.getHeight()) {
			textures[textures.size()].texture = glContext->create2DTexture(nullptr, 2048, 2048, gl::ImageFormat::RGBA);
			textures.at(textures.size()).texture.bufferSubData(0, 0, 1, 1, std::vector<unsigned char>{0, 0, 0, 0});
			textures.at(textures.size()).cursor.x = 1;
			textures.at(textures.size()).cursor.y = 0;
			textures.at(textures.size()).y_nextOffset = 1;

			cursor = textures.at(textures.size() - 1).cursor;
			y_plus = textures.at(textures.size() - 1).y_nextOffset;
			texture = textures.at(textures.size() - 1).texture;
		}

		texCoords.x = (0.5f + cursor.x) / float(texture.getWidth());
		texCoords.y = (0.5f + cursor.y) / float(texture.getHeight());

		texture.bufferSubData(cursor.x, cursor.y, image.getWidth(), image.getHeight(), *image.getData());
		cursor.x += image.getWidth();
		y_plus = std::max(y_plus, image.getHeight());

		texCoords.z = (0.5f + cursor.x) / float(texture.getWidth());
		texCoords.w = (0.5f + cursor.y + image.getHeight()) / float(texture.getHeight());

		return texCoords;
	}
	gml::Vec4f TextureManager::addTexture(const gl::Texture & image)
	{
		return addTexture(img::ImageReader(platform).readTexture(image));
	}
}
#include "texturemanager.h"

namespace gui 
{
	TextureManager::TextureManager(const std::shared_ptr<platform::IWindow> &platform, const std::shared_ptr<gl::ITextureCreator> &textureCreator)
		: platform(platform), textureCreator(textureCreator)
	{
        // create
		textures[0].texture = textureCreator->createTexture2D(TEXTURE_SIZE, TEXTURE_SIZE, gl::ImageFormat::RGBA, {});
        auto &textureAtlas = textures[0];

        // TODO: why do we need this emtpy space? Is this some kind of padding?
        textureAtlas.texture->setSubImage({0, 0, 2, 2}, std::vector<unsigned char>(16, 0));
        textureAtlas.cursor.x() = 2;
        textureAtlas.cursor.y() = 0;
        textureAtlas.y_nextOffset = 1;
	}

	gml::Vec4f TextureManager::addTexture(const img::Image & image)
	{
		gml::Vec2f& cursor = textures.at(textures.size() - 1).cursor;
		int& y_plus = textures.at(textures.size() - 1).y_nextOffset;
		gl::ITexture2D* texture = textures.at(textures.size() - 1).texture.get();

		gml::Vec4f texCoords;

		if (cursor.x() + image.getWidth() > texture->getWidth()) {
			cursor.x() = 0;
			cursor.y() += y_plus;
			y_plus = 0;
		}

		if (cursor.y() + image.getHeight() > texture->getHeight()) {
			textures[textures.size()].texture = textureCreator->createTexture2D(2048, 2048, gl::ImageFormat::RGBA, {});
            // TODO: is this padding?
			textures.at(textures.size()).texture->setSubImage({0, 0, 1, 1}, std::vector<unsigned char>{0, 0, 0, 0});
			textures.at(textures.size()).cursor.x() = 1;
			textures.at(textures.size()).cursor.y() = 0;
			textures.at(textures.size()).y_nextOffset = 1;

			cursor = textures.at(textures.size() - 1).cursor;
			y_plus = textures.at(textures.size() - 1).y_nextOffset;
			texture = textures.at(textures.size() - 1).texture.get();
		}

		texCoords.x() = (0.5f + cursor.x()) / float(texture->getWidth());
		texCoords.y() = (0.5f + cursor.y()) / float(texture->getHeight());

		texture->setSubImage({(int)cursor.x(), (int)cursor.y(), image.getWidth(), image.getHeight()}, {image.data(), image.getSize()});
		cursor.x() += image.getWidth();
		y_plus = std::max(y_plus, image.getHeight());

		texCoords.z() = (0.5f + cursor.x()) / float(texture->getWidth());
		texCoords.w() = (0.5f + cursor.y() + image.getHeight()) / float(texture->getHeight());

		return texCoords;
	}
	
	gml::Vec4f TextureManager::addTexture(gl::ITexture2D & texture)
	{
        auto data = texture.getImage();
		return addTexture(img::Image(texture.getWidth(), texture.getHeight(), texture.getChannels(), data));
	}
}
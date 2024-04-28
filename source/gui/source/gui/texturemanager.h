#pragma once
#include <map>
#include <vector>
#include "core.h"
#include "gml/gml.h"
#include "image/image.h"

namespace gui
{
	struct TextureAtlas
	{
		std::shared_ptr<gl::ITexture2D> texture; // TODO: unique_ptr and improve usage in renderer
		gml::Vec2f cursor;
		int y_nextOffset = 0;
	};

    /**
     * Stores the textures used by widgets, packed into atlases.
     *
     * Textures are packed into an atlas of fixed size. If an atlas is full, a new one is opened and added to an array
     * of atlases.
     */
	class TextureManager
	{
	private:
		std::shared_ptr<platform::IWindow> platform;
		std::shared_ptr<gl::ITextureCreator> textureCreator;

		std::map<int, TextureAtlas> textures;
		const int TEXTURE_SIZE = 2048;

	public:
		TextureManager(const std::shared_ptr<platform::IWindow> &platform, const std::shared_ptr<gl::ITextureCreator> &textureCreator);

		gml::Vec4f addTexture(const img::Image & image);

		gml::Vec4f addTexture(gl::ITexture2D & image);

		gml::Vec2f getAlphaTexCoords()
        {
            // TODO: what is this?
            return gml::Vec2f(0.5f/float(2048), 0.5f / float(2048));
        }

		std::shared_ptr<gl::ITexture2D> getTexture(int i)
        {
            return textures.at(i).texture;
        }
	};
}
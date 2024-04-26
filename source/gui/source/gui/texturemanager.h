#pragma once
#include <map>
#include <vector>
#include "core.h"
#include "math/gml.h"
#include "image/image.h"

namespace gui
{
	struct TextureAtlas
	{
		gl::Texture2D texture;
		gml::Vec2f cursor;
		int y_nextOffset = 0;
	};

	class TextureManager
	{
	private:
		sys::Window platform;
		gl::TextureCreator textureCreator;

		std::map<int, TextureAtlas> textures;
		const int TEXTURE_SIZE = 2048;

	public:
		TextureManager(sys::Window platform, gl::TextureCreator textureCreator);

		gml::Vec4f addTexture(const img::Image & image);
		gml::Vec4f addTexture(const gl::Texture2D & image);

		gml::Vec2f getAlphaTexCoords() { return gml::Vec2f(0.5f/float(2048), 0.5f / float(2048)); }
		gl::Texture2D getTexture(int i) { return textures.at(i).texture; }
	};
}
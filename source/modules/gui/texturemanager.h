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
		gl::Texture texture;
		gml::Vec2f cursor;
		int y_nextOffset = 0;
	};

	class TextureManager
	{
	private:
		sys::PlatformHandle* platform;
		gl::GraphicsContext* glContext;

		std::map<int, TextureAtlas> textures;
		const int TEXTURE_SIZE = 2048;

	public:
		TextureManager(sys::PlatformHandle* platform, gl::GraphicsContext* glContext);

		gml::Vec4f addTexture(const img::Image & image);
		gml::Vec4f addTexture(const gl::Texture & image);

		gml::Vec2f getAlphaTexCoords() const;
		gl::Texture getTexture(int i) const;
	};
}
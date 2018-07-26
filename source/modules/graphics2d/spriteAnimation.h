#pragma once

#include <vector>

#include <graphics/gl.h>

namespace gl2d
{
	class SpriteAnimation
	{
	private:
		std::vector<gl::Texture> textures;
		std::vector<gl::Drawable> drawables;
		int currentIndex = -1;

	public:
		SpriteAnimation() = default;
		SpriteAnimation(gl::Texture texture);

		gl::Texture getCurrentFrame();
	};
}

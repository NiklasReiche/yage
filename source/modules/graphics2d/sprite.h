#pragma once

#include <graphics/gl.h>
#include <math/vector.h>
#include "spriteAnimation.h"

namespace gl2d
{
	class Sprite
	{
	private:
		gml::Vec2f center;
		gml::Vec2f halfSize;

		gl::Drawable drawable;
		gl::Texture texture;

		SpriteAnimation animation;

	public:
		Sprite(gml::Vec2f center, gml::Vec2f halfSize, SpriteAnimation animation);
		Sprite(gml::Vec2f center, gml::Vec2f halfSize, gl::Texture texture, gl::GraphicsContext* glContext);

		gl::Texture getTexture();

		gml::Vec2f getCenter() const;
		gml::Vec2f getHalfSize() const;

		void setCenter(const gml::Vec2f& center);
		void setHalfSize(const gml::Vec2f& halfSize);
	};
}

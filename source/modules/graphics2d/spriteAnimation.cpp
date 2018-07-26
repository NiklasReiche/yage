#include "spriteAnimation.h"

gl2d::SpriteAnimation::SpriteAnimation(gl::Texture texture)
{
	textures.push_back(texture);
	currentIndex = 0;
}

gl::Texture gl2d::SpriteAnimation::getCurrentFrame()
{
	return textures.at(currentIndex);
}

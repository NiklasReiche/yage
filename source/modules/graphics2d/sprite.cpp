#include "sprite.h"

gl2d::Sprite::Sprite(gml::Vec2f center, gml::Vec2f halfSize, SpriteAnimation animation)
{
	this->animation = animation;
}

gl::Texture gl2d::Sprite::getTexture()
{
	return animation.getCurrentFrame();
}

gml::Vec2f gl2d::Sprite::getCenter() const
{
	return center;
}

void gl2d::Sprite::setCenter(const gml::Vec2f& center)
{
	this->center = center;
}

gml::Vec2f gl2d::Sprite::getHalfSize() const
{
	return halfSize;
}

void gl2d::Sprite::setHalfSize(const gml::Vec2f& halfSize)
{
	this->halfSize = halfSize;
}

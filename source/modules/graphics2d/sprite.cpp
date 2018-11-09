#include "sprite.h"

gl2d::Sprite::Sprite(gml::Vec2f center, gml::Vec2f halfSize, SpriteAnimation animation)
{
	this->animation = animation;
}

gl2d::Sprite::Sprite(gml::Vec2f center, gml::Vec2f halfSize, gl::Texture texture, gl::GraphicsContext* glContext)
{
	this->center = center;
	this->halfSize = halfSize;
	this->texture = texture;

	std::vector<float> vertices = {
		center.x - halfSize.x, center.y - halfSize.y, 0.0f, 0.0f,
		center.x - halfSize.x, center.y + halfSize.y, 0.0f, 1.0f,
		center.x + halfSize.x, center.y + halfSize.y, 1.0f, 1.0f,
		center.x + halfSize.x, center.y + halfSize.y, 1.0f, 1.0f,
		center.x + halfSize.x, center.y - halfSize.y, 1.0f, 0.0f,
		center.x - halfSize.x, center.y - halfSize.y, 1.0f, 0.0f
	};
	this->drawable = glContext->createDrawable(vertices, {2,2}, gl::DrawMode::DRAW_STATIC);
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

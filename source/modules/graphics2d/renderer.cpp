#include "renderer.h"

gl2d::Renderer::Renderer(gl::GraphicsContext * glContext)
{
	this->glContext = glContext;
}

void gl2d::Renderer::drawSprite(Sprite sprite)
{
	glContext->bindTexture(sprite.getTexture());
	glContext->draw(sprite.getDrawable());
}

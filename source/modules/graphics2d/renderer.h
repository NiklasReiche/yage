#pragma once

#include <graphics/graphics.h>
#include "sprite.h"

namespace gl2d
{
	class Renderer
	{
	private:
		gl::GraphicsContext glContext;
		
	public:
		void drawSprite(Sprite sprite);
	};
}
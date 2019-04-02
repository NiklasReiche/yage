#pragma once

#include <graphics/graphics.h>
#include "sprite.h"

namespace gl2d
{
	class Renderer
	{
	private:
		gl::GraphicsContext* glContext = nullptr;
		
	public:
		Renderer(gl::GraphicsContext* glContext);
		void drawSprite(Sprite sprite);
	};
}
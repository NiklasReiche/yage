#pragma once

#include "gl3.h"
#include "GL3_Object.h"
#include "GL3_Drawable.h"

namespace gl3
{
	class GL3_Framebuffer : public GL3_Object
	{
	private:
		friend class GL3_Context;

		int width, height;
		GLuint FBO;
		GLuint RBO;
		GLuint colorTexture;

	public:
		int getWidth() const { return width; }
		int getHeight() const { return height; }
	};
}
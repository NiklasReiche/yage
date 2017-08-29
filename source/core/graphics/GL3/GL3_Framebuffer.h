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

		GLuint FBO = 0;
		GLuint RBO = 0;
		GLuint colorTexture = 0;
		int width, height;

	public:
		GL3_Framebuffer();
		GL3_Framebuffer(const GL3_Framebuffer& other);
		~GL3_Framebuffer();
		GL3_Framebuffer& operator=(const GL3_Framebuffer& other);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
	};
}
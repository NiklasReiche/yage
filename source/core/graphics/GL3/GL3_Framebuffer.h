#pragma once

#include "gl3.h"
#include "GL3_Object.h"
#include "GL3_Drawable.h"
#include "GL3_Texture.h"

namespace gl3
{
	class GL3_Framebuffer : public GL3_Object
	{
	private:
		friend class GL3_Context;

		GLuint FBO = 0;
		GLuint RBO = 0;
		GL3_Texture texture;
		int width = 0;
		int height = 0;

	public:
		GL3_Framebuffer();
		GL3_Framebuffer(const GL3_Framebuffer& other);
		~GL3_Framebuffer();
		GL3_Framebuffer& operator=(const GL3_Framebuffer& other);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		GL3_Texture getTexture() const { return texture; }
	};
}

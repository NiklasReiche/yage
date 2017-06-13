#pragma once

#include "gles2.h"
#include "GLES2_Object.h"

namespace gles2
{
	class GLES2_Framebuffer : public GLES2_Object
	{
	private:
		friend class GLES2_Context;

		int width, height;
		GLuint FBO;
		GLuint RBO_D;
		GLuint RBO_S;
		GLuint colorTexture;

	public:
		int getWidth() const { return width; }
		int getHeight() const { return height; }
	};
}
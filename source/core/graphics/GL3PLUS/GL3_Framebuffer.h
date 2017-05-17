#pragma once

#include "gl3.h"

namespace gl3
{
	struct GL3_Framebuffer
	{
		int width, height;
		GLuint FBO;
		GLuint RBO;
		GLuint colorTexture;
	};
}
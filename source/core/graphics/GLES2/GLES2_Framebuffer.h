#pragma once

#include "gles2.h"
#include "GLES2_Object.h"

namespace gles2
{
	class GLES2_Framebuffer : public GLES2_Object
	{
	private:
		friend class GLES2_Context;

		GLuint FBO = 0;
		GLuint RBO_D = 0; // Depth-Buffer
		GLuint RBO_S = 0; // Stencil-Buffer
		GLuint colorTexture = 0;
		int width, height;

	public:
		GLES2_Framebuffer();
		GLES2_Framebuffer(const GLES2_Framebuffer& other);
		~GLES2_Framebuffer();
		GLES2_Framebuffer& operator=(const GLES2_Framebuffer& other);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
	};
}
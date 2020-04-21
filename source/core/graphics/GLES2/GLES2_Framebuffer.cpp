#include "GLES2_Framebuffer.h"

namespace gles2
{
	GLES2_Framebuffer::GLES2_Framebuffer()
		: GLES2_Object() {}
	GLES2_Framebuffer::GLES2_Framebuffer(const GLES2_Framebuffer& other)
		: GLES2_Object(other)
	{
		this->FBO = other.FBO;
		this->RBO_D = other.RBO_D;
		this->RBO_S = other.RBO_S;
		this->colorTexture = other.colorTexture;
		this->width = other.width;
		this->height = other.height;
	}
	GLES2_Framebuffer::~GLES2_Framebuffer()
	{
		if (*refCount == 1) {
			glDeleteBuffers(1, &FBO);
			glDeleteBuffers(1, &RBO_D);
			glDeleteBuffers(1, &RBO_S);
			glDeleteTextures(1, &colorTexture);
		}
	}

	GLES2_Framebuffer& GLES2_Framebuffer::operator=(const GLES2_Framebuffer& other)
	{
		GLES2_Object::operator=(other);
		if (shouldDelete) {
			glDeleteBuffers(1, &FBO);
			glDeleteBuffers(1, &RBO_D);
			glDeleteBuffers(1, &RBO_S);
			glDeleteTextures(1, &colorTexture);
			shouldDelete = false;
		}

		this->FBO = other.FBO;
		this->RBO_D = other.RBO_D;
		this->RBO_S = other.RBO_S;
		this->colorTexture = other.colorTexture;
		this->width = other.width;
		this->height = other.height;

		return *this;
	}
}
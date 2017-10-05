#include "GL3_Framebuffer.h"
#include "GL3_Context.h"

namespace gl3
{
	GL3_Framebuffer::GL3_Framebuffer()
		: GL3_Object() {}
	GL3_Framebuffer::GL3_Framebuffer(const GL3_Framebuffer& other)
		: GL3_Object(other)
	{
		this->FBO = other.FBO;
		this->RBO = other.RBO;
		this->texture = other.texture;
		this->width = other.width;
		this->height = other.height;
	}
	GL3_Framebuffer::~GL3_Framebuffer()
	{
		if (*refCount == 1) {
			glDeleteFramebuffers(1, &FBO);
			glDeleteRenderbuffers(1, &RBO);
		}
	}
	GL3_Framebuffer& GL3_Framebuffer::operator=(const GL3_Framebuffer& other)
	{
		GL3_Object::operator=(other);
		if (shouldDelete) {
			glDeleteFramebuffers(1, &FBO);
			glDeleteRenderbuffers(1, &RBO);
			shouldDelete = false;
		}

		this->FBO = other.FBO;
		this->RBO = other.RBO;
		this->texture = other.texture;
		this->width = other.width;
		this->height = other.height;

		return *this;
	}
}
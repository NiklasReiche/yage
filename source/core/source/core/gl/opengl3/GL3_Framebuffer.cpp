#include "GL3_Framebuffer.h"

#include "GL3_Context.h"

namespace gl3
{
	GL3_Framebuffer::~GL3_Framebuffer()
	{
		glDeleteFramebuffers(1, &FBO);
		glDeleteRenderbuffers(1, &RBO);
	}
	
	GL3_Framebuffer::GL3_Framebuffer(GL3_Framebuffer&& other) noexcept
		: GL3_Object(std::move(other))
	{
		this->FBO = other.FBO;
		this->RBO = other.RBO;
		this->texture = std::move(other.texture);
		this->width = other.width;
		this->height = other.height;

		other.FBO = 0;
		other.RBO = 0;
		other.width = 0;
		other.height = 0;
	}
	
	GL3_Framebuffer& GL3_Framebuffer::operator=(GL3_Framebuffer&& other) noexcept
	{
		if (this != &other)
		{
			GL3_Object::operator=(std::move(other));

			this->FBO = other.FBO;
			this->RBO = other.RBO;
			this->texture = std::move(other.texture);
			this->width = other.width;
			this->height = other.height;

			other.FBO = 0;
			other.RBO = 0;
			other.width = 0;
			other.height = 0;
		}

		return *this;
	}

	int GL3_Framebuffer::getWidth() const
	{
		return width;
	}
	
	int GL3_Framebuffer::getHeight() const
	{
		return height;
	}

	int GL3_Framebuffer::getChannels() const
	{
		return texture->getChannels();
	}
	
	std::shared_ptr<gl::ITexture2D> GL3_Framebuffer::getTexture() const
	{
		return texture;
	}
}
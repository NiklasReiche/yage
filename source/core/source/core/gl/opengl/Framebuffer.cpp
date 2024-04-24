#include "Framebuffer.h"

#include "Context.h"

namespace opengl
{
	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &FBO);
		glDeleteRenderbuffers(1, &RBO);
        lockContextPtr()->bindFramebuffer(GL_FRAMEBUFFER, 0); // TODO: make this a destroy method that just resets the bindings
	}

	Framebuffer::Framebuffer(Framebuffer&& other) noexcept
		: OpenGlObject(std::move(other))
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

	Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
	{
		if (this != &other)
		{
			OpenGlObject::operator=(std::move(other));

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

	int Framebuffer::getWidth() const
	{
		return width;
	}
	
	int Framebuffer::getHeight() const
	{
		return height;
	}

	int Framebuffer::getChannels() const
	{
		return texture->getChannels();
	}
	
	std::shared_ptr<gl::ITexture2D> Framebuffer::getTexture() const
	{
		return texture;
	}
}
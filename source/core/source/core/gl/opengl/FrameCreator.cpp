#include "FrameCreator.h"

#include "Context.h"
#include "Framebuffer.h"
#include "TextureCreator.h"

#include "utils/NotImplementedException.h"

namespace opengl
{
	std::unique_ptr<gl::IFrame> FrameCreator::createFrame(
		const int width,
		const int height,
		const gl::ImageFormat format)
	{
		auto context = lockContextPtr();
		auto buffer = std::unique_ptr<Framebuffer>(new Framebuffer(context));

		// generate gpu object
		glGenFramebuffers(1, &buffer->FBO);
		context->bindFramebuffer(GL_FRAMEBUFFER, buffer->FBO);

		// create empty texture
		const std::span<unsigned char> emptyData;
		buffer->texture = std::static_pointer_cast<Texture2D>(std::shared_ptr<gl::ITexture2D>(context->getTextureCreator()->
			createTexture2D(width, height, format, emptyData)));
		buffer->texture->configTextureFilter(gl::TextureFilter::LINEAR, gl::TextureFilter::LINEAR);

		// attach texture
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, buffer->texture->getId(), 0);

		// attach depth & stencil buffers
		glGenRenderbuffers(1, &buffer->RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, buffer->RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->RBO);

		// completeness check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			// TODO: throw proper exception
			throw;
		}

		// construct object
		buffer->id = buffer->FBO;

		buffer->height = height;
		buffer->width = width;
		return buffer;
	}

	std::shared_ptr<gl::IFrame> FrameCreator::getDefaultFrame()
	{
		throw NotImplementedException();
	}
}

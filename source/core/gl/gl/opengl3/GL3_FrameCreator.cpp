#include "GL3_FrameCreator.h"

#include "GL3_Context.h"
#include "GL3_Framebuffer.h"
#include "GL3_TextureCreator.h"

#include "utils/NotImplementedException.h"

namespace gl3
{
	std::unique_ptr<gl::IFrame> GL3_FrameCreator::createFrame(
		const int width,
		const int height,
		const gl::ImageFormat format)
	{
		auto context = lockContextPtr();
		auto buffer = std::unique_ptr<GL3_Framebuffer>(new GL3_Framebuffer(context));

		// generate gpu object
		glGenFramebuffers(1, &buffer->FBO);
		context->bindFramebuffer(GL_FRAMEBUFFER, buffer->FBO);

		// create empty texture
		const std::vector<unsigned char> emptyData = {};
		buffer->texture = std::static_pointer_cast<GL3_Texture2D>(std::shared_ptr<gl::ITexture2D>(context->getTextureCreator()->
			createTexture2D(width, height, format, emptyData)));
		buffer->texture->configTextureFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);

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
			std::cout << "Error" << std::endl;
			throw;
		}

		// construct object
		buffer->id = buffer->FBO;

		buffer->height = height;
		buffer->width = width;
		return buffer;
	}

	std::shared_ptr<gl::IFrame> GL3_FrameCreator::getDefaultFrame()
	{
		throw NotImplementedException();
	}
}

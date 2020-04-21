#include "GL3_Texture.h"
#include "GL3_Context.h"
#include "GL3_Framebuffer.h"

namespace gl3
{
	GL3_Texture::~GL3_Texture()
	{
		glDeleteTextures(1, &texture);
	}

	GL3_Texture::GL3_Texture(GL3_Texture&& other) noexcept
		: GL3_Object(std::move(other))
	{
		*this = std::move(other);
	}

	GL3_Texture& GL3_Texture::operator=(GL3_Texture&& other) noexcept
	{
		if (this != &other)
		{
			GL3_Object::operator=(std::move(other));

			this->texture = other.texture;
			this->target = other.target;
			this->format = other.format;
			this->pxFormat = other.pxFormat;
			this->pxType = other.pxType;
			this->rowAlignment = other.rowAlignment;
			this->nChannels = other.nChannels;

			other.texture = 0;
			other.target = TextureType();
			other.format = InternalFormat();
			other.pxFormat = ImageFormat();
			other.pxType = PixelType();
			other.rowAlignment = 4;
			other.nChannels = 0;
		}

		return *this;
	}
	
	// ReSharper disable once CppMemberFunctionMayBeConst
	void GL3_Texture::configTextureWrapper(TextureWrapper xOption, TextureWrapper yOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_S, static_cast<GLenum>(xOption));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_T, static_cast<GLenum>(yOption));
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void GL3_Texture::configTextureFilter(TextureFilter minOption, TextureFilter magOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minOption));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magOption));
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void GL3_Texture::configTextureFilter(MipmapOption minOption, TextureFilter magOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

		glGenerateMipmap(static_cast<GLenum>(target));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minOption));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magOption));
	}

	void GL3_Texture::resize(int width, int height)
	{
#if 0
		GLuint current_fbo = glContext->getGlState().renderTarget;
		Viewport current_viewport = glContext->getGlState().viewport;

		std::shared_ptr<GL3_Framebuffer> fbo = glContext->createFrame(width, height, px_format);

		glContext->setActiveRenderTarget(fbo);
		glContext->setActiveViewport(Viewport(0, 0, width, height));
		glContext->draw(*this);

		operator=(fbo->getTexture());

		glContext->setActiveRenderTarget(current_fbo);
		glContext->setActiveViewport(current_viewport);
#endif
	}
	void GL3_Texture::rotate(int level)
	{
#if 0
		GLuint current_fbo = glContext->getGlState().renderTarget;
		Viewport current_viewport = glContext->getGlState().viewport;

		GL3_Framebuffer fbo = glContext->createFramebuffer(width, height, px_format);

		glContext->setActiveRenderTarget(fbo);
		glContext->setActiveViewport(Viewport(0, 0, width, height));

		glContext->useUnitTransformShader(gml::Mat4f::axisAngle(gml::Vec3f(0.0f, 0.0f, 1.0f), level * 90));
		glContext->bindTexture(shared_from_this());
		glContext->drawUnitDrawable();

		operator=(fbo.getTexture());

		glContext->setActiveRenderTarget(current_fbo);
		glContext->setActiveViewport(current_viewport);
#endif
	}
}

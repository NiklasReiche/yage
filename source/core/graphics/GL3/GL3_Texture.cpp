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
			this->nChannels = other.nChannels;

			other.texture = 0;
			other.target = TextureType();
			other.format = InternalFormat();
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
}

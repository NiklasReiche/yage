#include "Texture.h"
#include "Context.h"
#include "Framebuffer.h"

namespace opengl
{
	Texture::~Texture()
	{
		glDeleteTextures(1, &texture);
	}

	Texture::Texture(Texture&& other) noexcept
		: OpenGlObject(std::move(other))
	{
		*this = std::move(other);
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			OpenGlObject::operator=(std::move(other));

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
	void Texture::configTextureWrapper(TextureWrapper xOption, TextureWrapper yOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_S, static_cast<GLenum>(xOption));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_T, static_cast<GLenum>(yOption));
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void Texture::configTextureFilter(TextureFilter minOption, TextureFilter magOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minOption));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magOption));
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void Texture::configTextureFilter(MipmapOption minOption, TextureFilter magOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

		glGenerateMipmap(static_cast<GLenum>(target));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(minOption));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(magOption));
	}
}

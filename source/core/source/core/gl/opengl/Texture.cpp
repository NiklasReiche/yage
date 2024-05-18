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
	void Texture::configTextureWrapper(gl::TextureWrapper xOption, gl::TextureWrapper yOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

        m_wrap_s = convertWrapper(xOption);
        m_wrap_t = convertWrapper(yOption);

		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_S, static_cast<GLint>(m_wrap_s));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_T, static_cast<GLint>(m_wrap_t));
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void Texture::configTextureFilter(gl::TextureFilter minOption, gl::TextureFilter magOption)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(target), texture);

        m_filter_min = convertFilter(minOption);
        m_filter_mag = convertFilter(magOption);

		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_filter_min));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_filter_mag));
	}

    bool Texture::requires_mipmaps() const
    {
        switch (m_filter_min) {
            case TextureFilter::NEAREST_MIPMAP_NEAREST:
            case TextureFilter::NEAREST_MIPMAP_LINEAR:
            case TextureFilter::LINEAR_MIPMAP_NEAREST:
            case TextureFilter::LINEAR_MIPMAP_LINEAR:
                return true;
            default:
                break;
        }
        switch (m_filter_mag) {
            case TextureFilter::NEAREST_MIPMAP_NEAREST:
            case TextureFilter::NEAREST_MIPMAP_LINEAR:
            case TextureFilter::LINEAR_MIPMAP_NEAREST:
            case TextureFilter::LINEAR_MIPMAP_LINEAR:
                return true;
            default:
                break;
        }
        return false;
    }
}

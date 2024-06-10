#pragma once

#include "OpenGL.h"

#include "GlEnum.h"
#include "OpenGlObject.h"

namespace yage::opengl
{
	class Texture : public OpenGlObject
	{
	public:
		virtual ~Texture();
		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;

        void configTextureWrapper(gl::TextureWrapper xOption, gl::TextureWrapper yOption);

        void configTextureFilter(gl::TextureFilter minOption, gl::TextureFilter magOption);

        [[nodiscard]]
        bool requires_mipmaps() const;

	protected:
		const GLenum pxType = GL_UNSIGNED_BYTE;
		GLuint texture = 0;
		TextureType target = TextureType::UNDEFINED;
		InternalFormat format = InternalFormat::UNDEFINED;
		int nChannels = 0;
        TextureWrapper m_wrap_s;
        TextureWrapper m_wrap_t;
        TextureFilter m_filter_min;
        TextureFilter m_filter_mag;

		using OpenGlObject::OpenGlObject;

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

	private:
		friend class Context;
		friend class Renderer;
		friend class TextureCreator;
	};
}

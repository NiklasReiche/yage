#pragma once

#include "OpenGL.h"

#include "GlEnum.h"
#include "OpenGlObject.h"

namespace opengl
{
	class Texture : public OpenGlObject
	{
	public:
		virtual ~Texture();
		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;

		void configTextureWrapper(TextureWrapper xOption, TextureWrapper yOption);
		void configTextureFilter(TextureFilter minOption, TextureFilter magOption);
		void configTextureFilter(MipmapOption minOption, TextureFilter magOption);

	protected:
		const GLenum pxType = GL_UNSIGNED_BYTE;
		GLuint texture = 0;
		TextureType target = TextureType::UNDEFINED;
		InternalFormat format = InternalFormat::UNDEFINED;
		int nChannels = 0;

		using OpenGlObject::OpenGlObject;

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

	private:
		friend class Context;
		friend class Renderer;
		friend class TextureCreator;
	};
}

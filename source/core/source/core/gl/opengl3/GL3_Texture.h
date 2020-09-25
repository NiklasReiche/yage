#pragma once

#include "OpenGL.h"

#include "GL3_Object.h"
#include "GL3_Enum.h"

namespace gl3
{
	class GL3_Texture : public GL3_Object
	{
	public:
		virtual ~GL3_Texture();
		GL3_Texture(const GL3_Texture& other) = delete;
		GL3_Texture& operator=(const GL3_Texture& other) = delete;

		void configTextureWrapper(TextureWrapper xOption, TextureWrapper yOption);
		void configTextureFilter(TextureFilter minOption, TextureFilter magOption);
		void configTextureFilter(MipmapOption minOption, TextureFilter magOption);

	protected:
		const GLenum pxType = GL_UNSIGNED_BYTE;
		GLuint texture = 0;
		TextureType target = TextureType::UNDEFINED;
		InternalFormat format = InternalFormat::UNDEFINED;
		int nChannels = 0;

		using GL3_Object::GL3_Object;

		GL3_Texture(GL3_Texture&& other) noexcept;
		GL3_Texture& operator=(GL3_Texture&& other) noexcept;

	private:
		friend class GL3_Context;
		friend class GL3_Renderer;
		friend class GL3_TextureCreator;
	};
}

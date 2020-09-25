#pragma once

#include "../Frame.h"

#include "OpenGL.h"
#include "GL3_Object.h"
#include "GL3_Texture2D.h"

namespace gl3
{
	class GL3_Framebuffer final : public GL3_Object, public gl::IFrame
	{
	public:
		virtual ~GL3_Framebuffer();
		GL3_Framebuffer(const GL3_Framebuffer& other) = delete;
		GL3_Framebuffer& operator=(const GL3_Framebuffer& other) = delete;

		[[nodiscard]]
		int getWidth() const override;
		[[nodiscard]]
		int getHeight() const override;
		[[nodiscard]]
		int getChannels() const override;
		[[nodiscard]]
		std::shared_ptr<gl::ITexture2D> getTexture() const override;

	private:
		GLuint FBO = 0;
		GLuint RBO = 0;
		std::shared_ptr<GL3_Texture2D> texture;
		int width = 0;
		int height = 0;

		using GL3_Object::GL3_Object;

		GL3_Framebuffer(GL3_Framebuffer&& other) noexcept;
		GL3_Framebuffer& operator=(GL3_Framebuffer&& other) noexcept;

		friend class GL3_Context;
		friend class GL3_Renderer;
		friend class GL3_FrameCreator;
	};
}

#pragma once

#include "../Frame.h"

#include "OpenGlObject.h"
#include "OpenGL.h"
#include "Texture2D.h"

namespace yage::opengl
{
	class Framebuffer final : public OpenGlObject, public gl::IFrame
	{
	public:
		virtual ~Framebuffer();
		Framebuffer(const Framebuffer& other) = delete;
		Framebuffer& operator=(const Framebuffer& other) = delete;

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
		std::shared_ptr<Texture2D> texture;
		int width = 0;
		int height = 0;

		using OpenGlObject::OpenGlObject;

		Framebuffer(Framebuffer&& other) noexcept;
		Framebuffer& operator=(Framebuffer&& other) noexcept;

		friend class Context;
		friend class Renderer;
		friend class FrameCreator;
	};
}

#pragma once

#include <memory>

#include "../Renderer.h"
#include "GL3_Texture.h"

namespace gl3
{
	class GL3_Context;
	
	class GL3_Renderer : public GL3_BaseObject, public gl::IRenderer
	{
	public:
		void clear() override;

		void setClearColor(uint32_t color) override;
		
		void setRenderTarget(const gl::IFrame& target) override;

		void setViewport(int x, int y, int width, int height) override;
		void setViewport(Viewport viewport) override;
		
		void draw(const gl::IDrawable & drawable) override;
		void draw(const gl::IFrame & buffer) override;
		void draw(const gl::ITexture2D & texture);

		void useShader(const gl::IShader & shader) override;

		void bindTexture(const gl::ITexture2D & texture, int unit = 0) override;
		void bindTexture(const gl::ITexture3D & texture, int unit = 0) override;
		void bindTexture(const gl::ICubemap & texture, int unit = 0) override;

		void enableDepthTest() override;
		void disableDepthTest() override;
		void setDepthTest(bool value) override;

		void enableStencilTest() override;
		void disableStencilTest() override;
		void setStencilTest(bool value) override;

		void enableBlending() override;
		void disableBlending() override;
		void setBlending(bool value) override;

		void enableWireframe() override;
		void disableWireframe() override;
		void setWireframe(bool value) override;

	private:
		gml::Vec4f clearColor = gml::Vec4f(0, 0, 0, 1);
		
		static std::unique_ptr<gl::IDrawable> unitDrawable;
		static std::unique_ptr<gl::IShader> unitShader;

		explicit GL3_Renderer(std::weak_ptr<GL3_Context> contextPtr);

		void bindTexture(const GL3_Texture& texture, int unit);

		friend class GL3_Context;
	};
}

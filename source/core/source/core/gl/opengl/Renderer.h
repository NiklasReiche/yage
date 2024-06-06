#pragma once

#include <memory>

#include "../Renderer.h"
#include "Texture.h"

namespace opengl
{
	class Context;
	
	class Renderer : public BaseObject, public gl::IRenderer
	{
	public:
		void clear() override;

		void setClearColor(uint32_t color) override;
		
		void setRenderTarget(const gl::IFrame& target) override;
        void setDefaultRenderTarget() override;

		void setViewport(int x, int y, int width, int height) override;
		void setViewport(Viewport viewport) override;
		
		void draw(const gl::IDrawable & drawable) override;
		void draw(const gl::IFrame & buffer) override;
		void draw(const gl::ITexture2D & texture) override;

		void useShader(const gl::IShader & shader) override;

		void bindTexture(const gl::ITexture2D & texture, int unit = 0) override;
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

        void enablePointSize() override;
        void disablePointSize() override;

	private:
		gml::Vec4f clearColor = gml::Vec4f(0, 0, 0, 1);

        std::unique_ptr<gl::IDrawable> unitDrawable;
        std::unique_ptr<gl::IShader> unitShader;

		explicit Renderer(std::weak_ptr<Context> contextPtr);

		void bindTexture(const Texture& texture, int unit);

		friend class Context;
	};
}

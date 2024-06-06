#pragma once

#include "Drawable.h"
#include "Shader.h"
#include "Frame.h"
#include "Texture2D.h"
#include "Cubemap.h"

namespace gl
{
	class IRenderer
	{
	public:
		struct Viewport
		{
			int x = -1, y = -1;
			int width = -1, height = -1;
		};

		virtual ~IRenderer() = default;

		virtual void clear() = 0;
		
		virtual void setClearColor(uint32_t color) = 0;

		virtual void setRenderTarget(const IFrame& target) = 0;
        virtual void setDefaultRenderTarget() = 0;
		
		virtual void setViewport(int x, int y, int width, int height) = 0;
		virtual void setViewport(Viewport viewport) = 0;

		virtual void draw(const IDrawable& drawable) = 0;
		virtual void draw(const IFrame& buffer) = 0;
        virtual void draw(const ITexture2D& texture) = 0;

		virtual void useShader(const IShader& shader) = 0;

		virtual void bindTexture(const ITexture2D& texture, int unit = 0) = 0;
		virtual void bindTexture(const ICubemap& texture, int unit = 0) = 0;

		virtual void enableDepthTest() = 0;
		virtual void disableDepthTest() = 0;
		virtual void setDepthTest(bool value) = 0;

		virtual void enableStencilTest() = 0;
		virtual void disableStencilTest() = 0;
		virtual void setStencilTest(bool value) = 0;

		virtual void enableBlending() = 0;
		virtual void disableBlending() = 0;
		virtual void setBlending(bool value) = 0;

		virtual void enableWireframe() = 0;
		virtual void disableWireframe() = 0;
		virtual void setWireframe(bool value) = 0;

        virtual void enablePointSize() = 0;
        virtual void disablePointSize() = 0;

	protected:
		IRenderer() = default;
		IRenderer(const IRenderer& other) = default;
		IRenderer(IRenderer&& other) = default;
		IRenderer& operator=(const IRenderer & other) = default;
		IRenderer& operator=(IRenderer&& other) = default;
	};
}

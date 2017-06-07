#pragma once

#include "gl.h"
#include "Color.h"

namespace gl
{
	class Renderer
	{
	private:
		GraphicsContext* glContext;

	protected:
		Viewport default_viewport;
		Framebuffer default_framebuffer;
		Viewport framebuffer_viewport;
		gml::Vec4<float> clearColor = gml::Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);

	public:
		Renderer() {}
		Renderer(GraphicsContext* glContext, Viewport viewport)
			: glContext(glContext), default_viewport(viewport)
		{
			default_framebuffer = glContext->createFramebuffer(viewport.width, viewport.height);
			framebuffer_viewport = Viewport(0, 0, default_framebuffer.getWidth(), default_framebuffer.getHeight());
		}

		virtual void render()
		{
			prepareRenderTarget();

			renderToScreen();
		}

		void prepareRenderTarget()
		{
			glContext->setActiveRenderTarget(default_framebuffer);
			glContext->setActiveViewport(framebuffer_viewport);

			glClearColor(clearColor.x, clearColor.z, clearColor.z, clearColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void renderToScreen()
		{
			glContext->setActiveViewport(default_viewport);
			glContext->draw(default_framebuffer);
		}
	};
}
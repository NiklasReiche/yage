#include "Renderer.h"

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	GuiRenderer::GuiRenderer(platform::PlatformHandle * platform, gl::GraphicsContext* glContext, gl::Viewport viewport)
		: Renderer(glContext, viewport), platform(platform)
	{
#ifdef DESKTOP
		gl::ShaderLoader loader(glContext);
		guiShader = loader.loadShader("D:/Dev/Projects/YAGE/source/modules/gui/shaders/guiShader_GL3.vert", "D:/Dev/Projects/YAGE/source/modules/gui/shaders/guiShader_GL3.frag");
		textShader = loader.loadShader("D:/Dev/Projects/YAGE/source/modules/font/shaders/textShader_GL3.vert", "D:/Dev/Projects/YAGE/source/modules/font/shaders/textShader_GL3.frag");
#endif
#ifdef ANDROID
        gl::ShaderLoader loader(platform, glContext);
		guiShader = loader.loadShader("assets/shaders/gui/guiShader_ES1.vert", "assets/shaders/gui/guiShader_ES1.frag");
		textShader = loader.loadShader("assets/shaders/font/textShader_ES1.vert", "assets/shaders/font/textShader_ES1.frag");
#endif

		gml::Matrix4D<float> projection = gml::orthographic<float>(0.0f, (float)viewport.width, (float)viewport.height, 0.0f, 0.1f, 100.0f);
		guiShader.setUniform("projection", projection);
		textShader.setUniform("projection", projection);

		clearColor = gml::Vec4<float>(1, 1, 1, 1);
	}


	void GuiRenderer::render()
	{
		prepareRenderTarget();
		glContext->enableBlending();

		glContext->useShader(guiShader);
		for (unsigned int i = 0; i < widgets.size(); ++i)
		{
			glContext->draw(*widgets[i]);
		}
		
		glContext->useShader(textShader);
		for (unsigned int i = 0; i < text.size(); ++i)
		{
			glContext->bindTexture(text[i]->getTexture(), 0);
			glContext->draw(*text[i]);
		}

		glContext->disableBlending();
		renderToScreen();
	}

	void GuiRenderer::drawWidget(gl::Drawable & widget)
	{
		glContext->useShader(guiShader);
		glContext->draw(widget);
	}

	void GuiRenderer::drawText(font::Text & text)
	{
		glContext->enableBlending();
		glContext->useShader(textShader);

		glContext->bindTexture(text.getTexture(), 0);
		glContext->draw(text);

		glContext->disableBlending();
	}
}
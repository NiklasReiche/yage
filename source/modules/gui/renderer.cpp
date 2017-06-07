#include "renderer.h"

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	GuiRenderer::GuiRenderer(gl::GraphicsContext* glContext, gl::Viewport viewport)
		: Renderer(glContext, viewport)
	{
		gl::ShaderLoader loader(glContext);
		guiShader = loader.loadShader("D:/Dev/Projects/YAGE/source/modules/gui/guiShader.vert", "D:/Dev/Projects/YAGE/source/modules/gui/guiShader.frag");
		textShader = loader.loadShader("D:/Dev/Projects/YAGE/source/modules/font/textShader.vert", "D:/Dev/Projects/YAGE/source/modules/font/textShader.frag");

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

		//ngl::enableBlending();
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
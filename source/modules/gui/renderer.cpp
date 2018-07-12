#include "renderer.h"
#include "shaders.h"

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	GuiRenderer::GuiRenderer(sys::PlatformHandle * platform, gl::GraphicsContext* glContext, gl::Viewport viewport)
		: Renderer(glContext, viewport), platform(platform)
	{
		gui::ShaderTemplate guiShaderTemplate;
		font::ShaderTemplate fontShaderTemplate;

		gl::ShaderLoader loader(platform, glContext);

		guiShader = loader.loadFromString(guiShaderTemplate.guiShader_vert, guiShaderTemplate.guiShader_frag);
		textShader = loader.loadFromString(fontShaderTemplate.textShader_vert, fontShaderTemplate.textShader_frag);

		gml::Mat4d projection = gml::Mat4d::orthographic(0.0, viewport.width, viewport.height, 0.0, 0.1, 100.0);
		guiShader.setUniform("projection", projection);
		guiShader.setUniform("texture", 0);
		textShader.setUniform("projection", projection);

		clearColor = gml::Vec4<float>(1, 1, 1, 1);
	}


	void GuiRenderer::render()
	{
		prepareRenderTarget();
		glContext->enableBlending();

		glContext->useShader(guiShader);
		glContext->bindTexture(guiTexture);
		for (auto widget : widgets)
		{
			glContext->draw(*widget);
		}

		glContext->useShader(textShader);
		for (auto texts : text)
		{
			glContext->bindTexture(texts->getTexture(), 0);
			glContext->draw(*texts);
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

	void GuiRenderer::setTexture(gl::Texture texture)
	{
		guiTexture = texture;
	}
}

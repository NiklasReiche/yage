#include "Renderer.h"
#include "Shaders.h"

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

		gml::Matrix4D<float> projection = gml::orthographic<float>(0.0f, (float)viewport.width, (float)viewport.height, 0.0f, 0.1f, 100.0f);
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

	void GuiRenderer::setTexture(gl::Texture texture)
	{
		guiTexture = texture;
	}
}
#include "renderer.h"
#include "shaders.h"

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	GuiRenderer::GuiRenderer(gl::Renderer glRenderer, gl::ShaderCreator shaderCreator, gl::IRenderer::Viewport viewport)
		: glRenderer(glRenderer)
	{
		glRenderer->setViewport(viewport);
		
		gui::ShaderTemplate guiShaderTemplate;
		font::ShaderTemplate fontShaderTemplate;

		guiShader = shaderCreator->createShader(guiShaderTemplate.guiShader_vert, guiShaderTemplate.guiShader_frag);
		textShader = shaderCreator->createShader(fontShaderTemplate.textShader_vert, fontShaderTemplate.textShader_frag);

		gml::Mat4d projection = gml::Mat4d::orthographic(0.0, (float)viewport.width, (float)viewport.height, 0.0, 0.1, 100.0);
		guiShader->setUniform("projection", projection);
		guiShader->setUniform("texture", 0);
		textShader->setUniform("projection", projection);

		//clearColor = gml::Vec4<float>(1, 1, 1, 1);
	}


	void GuiRenderer::render()
	{
		//prepareRenderTarget();
		glRenderer->enableBlending();

		glRenderer->useShader(guiShader);
		glRenderer->bindTexture(guiTexture);
		for (unsigned int i = 0; i < widgets.size(); ++i)
		{
			glRenderer->draw(widgets[i]);
		}

		glRenderer->useShader(textShader);
		for (unsigned int i = 0; i < text.size(); ++i)
		{
			glRenderer->bindTexture(text[i]->getTexture(), 0);
			glRenderer->draw(text[i]->getDrawable());
		}

		glRenderer->disableBlending();
		//renderToScreen();
	}

	void GuiRenderer::setTexture(gl::Texture2D texture)
	{
		guiTexture = texture;
	}
}

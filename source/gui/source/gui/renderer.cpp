#include "renderer.h"
#include "shaders.h"
#include <font/shaders.h>

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	GuiRenderer::GuiRenderer(const std::shared_ptr<gl::IRenderer> &glRenderer,
                             const std::shared_ptr<gl::IShaderCreator> &shaderCreator, const gl::IRenderer::Viewport &viewport)
		: glRenderer(glRenderer)
	{
		glRenderer->setViewport(viewport);
		
		gui::ShaderTemplate guiShaderTemplate;

		guiShader = shaderCreator->createShader(guiShaderTemplate.guiShader_vert, guiShaderTemplate.guiShader_frag);
		textShader = shaderCreator->createShader(font::shaders::TextShader::vert, font::shaders::TextShader::frag);

		gml::Mat4d projection = gml::matrix::orthographic<float>(0.0, (float)viewport.width, (float)viewport.height, 0.0, 0.1, 100.0);
		guiShader->setUniform("projection", projection);
		guiShader->setUniform("texture", 0);
		textShader->setUniform("projection", projection);

		//clearColor = gml::Vec4<float>(1, 1, 1, 1);
	}


	void GuiRenderer::render()
	{
		//prepareRenderTarget();
        glRenderer->setClearColor(gl::Color::WHITE);
        glRenderer->clear();
		glRenderer->enableBlending();

		glRenderer->useShader(*guiShader);
		glRenderer->bindTexture(*guiTexture);
		for (unsigned int i = 0; i < widgets.size(); ++i)
		{
			glRenderer->draw(*widgets[i]);
		}

		glRenderer->useShader(*textShader);
		for (unsigned int i = 0; i < text.size(); ++i)
		{
			glRenderer->bindTexture(text[i]->getTexture(), 0);
			glRenderer->draw(text[i]->getDrawable());
		}

		glRenderer->disableBlending();
		//renderToScreen();
	}

	void GuiRenderer::setTexture(std::shared_ptr<gl::ITexture2D> texture)
	{
		guiTexture = texture;
	}
}

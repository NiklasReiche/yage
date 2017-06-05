#include "renderer.h"

using namespace std;
using namespace gml;


namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	GuiRenderer::GuiRenderer(ngl::Viewport viewport)
		: Renderer(viewport)
	{
		ngl::ShaderLoader loader;
		guiShader = loader.loadShader("../NE Gui Library/shader/guiShader.vert", "../NE Gui Library/shader/guiShader.frag");
		textShader = loader.loadShader("../NE Font Library/shader/textShader.vert", "../NE Font Library/shader/textShader.frag");

		Matrix4D<float> projection = gml::orthographic<float>(0.0f, (float)viewport.width, (float)viewport.height, 0.0f, 0.1f, 100.0f);
		ngl::setUniform(guiShader, "projection", projection);
		ngl::setUniform(textShader, "projection", projection);

		default_framebuffer.clearColor = gml::Vec4<float>(1, 1, 1, 1);
	}


	void GuiRenderer::render()
	{
		prepareRenderTarget();
		ngl::enableBlending();

		ngl::useShader(guiShader);
		for (unsigned int i = 0; i < widgets.size(); ++i)
		{
			ngl::draw(*widgets[i]);
		}

		//ngl::enableBlending();
		ngl::useShader(textShader);
		for (unsigned int i = 0; i < text.size(); ++i)
		{
			ngl::bindTexture(text[i]->getTexture(), 0);
			ngl::draw(*text[i]);
		}

		ngl::disableBlending();
		renderToScreen();
	}

	void GuiRenderer::drawWidget(ngl::Drawable & widget)
	{
		ngl::useShader(guiShader);
		ngl::draw(widget);
	}

	void GuiRenderer::drawText(font::Text & text)
	{
		ngl::enableBlending();
		ngl::useShader(textShader);

		ngl::bindTexture(text.getTexture(), 0);
		ngl::draw(text);

		ngl::disableBlending();
	}
}
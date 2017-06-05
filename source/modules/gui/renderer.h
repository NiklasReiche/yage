#pragma once

#include <vector>
#include "core.h"


namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	class GuiRenderer : public ngl::Renderer
	{
	private:
		ngl::Shader guiShader;
		ngl::Shader textShader;
	public:
		std::vector<ngl::Drawable*> widgets;
		std::vector<font::Text*> text;

		GuiRenderer() {}
		GuiRenderer(ngl::Viewport viewport);

		void render();

		void drawWidget(ngl::Drawable & widget);
		void drawText(font::Text & text);
	};
}
#pragma once

#include <vector>
#include "core.h"
#include "font/Shaders.h"

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	class GuiRenderer : public gl::Renderer
	{
	private:
		sys::PlatformHandle* platform;
		gl::Shader guiShader;
		gl::Shader textShader;

	public:
		std::vector<gl::Drawable*> widgets;
		std::vector<font::Text*> text;

		GuiRenderer() {}
		GuiRenderer(sys::PlatformHandle * platform, gl::GraphicsContext* glContext, gl::Viewport viewport);

		void render();

		void drawWidget(gl::Drawable & widget);
		void drawText(font::Text & text);
	};
}
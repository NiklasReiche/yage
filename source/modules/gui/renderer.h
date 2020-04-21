#pragma once

#include <vector>
#include "core.h"
#include "font/shaders.h"
#include "image/image.h"

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	class GuiRenderer
	{
	public:
		std::vector<gl::Drawable> widgets;
		std::vector<font::Text*> text;

		GuiRenderer() {}
		GuiRenderer(gl::Renderer glContext, gl::ShaderCreator s, gl::IRenderer::Viewport viewport);

		void render();

		void setTexture(gl::Texture2D texture);

	private:
		gl::Renderer glRenderer;
		gl::Shader guiShader;
		gl::Shader textShader;
		gl::Texture2D guiTexture;
	};
}
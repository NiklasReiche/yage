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
		std::vector<gl::IDrawable*> widgets;
		std::vector<font::Text*> text;

		GuiRenderer() = default;
		GuiRenderer(const std::shared_ptr<gl::IRenderer> &glRenderer,
                    const std::shared_ptr<gl::IShaderCreator> &shaderCreator,
                    const gl::IRenderer::Viewport &viewport);

		void render();

        // TODO: this is probably not good
		void setTexture(std::shared_ptr<gl::ITexture2D> texture);

	private:
        std::shared_ptr<gl::IRenderer> glRenderer;
        std::unique_ptr<gl::IShader> guiShader;
        std::unique_ptr<gl::IShader> textShader;
        std::shared_ptr<gl::ITexture2D> guiTexture; // TODO: don't have this as a field
	};
}
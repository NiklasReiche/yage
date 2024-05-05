#pragma once

#include <vector>
#include "core.h"
#include "font/shaders.h"
#include "image/image.h"
#include "root.h"

namespace gui
{
	class GuiRenderer
	{
	public:
		GuiRenderer() = delete;

		GuiRenderer(const std::shared_ptr<gl::IRenderer> &glRenderer,
                    const std::shared_ptr<gl::IShaderCreator> &shaderCreator,
                    const gl::IRenderer::Viewport &viewport);

        void render(RootWidget& root);

	private:
        std::shared_ptr<gl::IRenderer> glRenderer;
        std::unique_ptr<gl::IShader> guiShader;
        std::unique_ptr<gl::IShader> textShader;

        void
        collect_drawables(std::vector<Widget*>& vector_widget, std::vector<font::Text*>& vector_text, Widget& widget);
	};
}
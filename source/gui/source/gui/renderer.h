#pragma once

#include <vector>

#include <font/shaders.h>
#include <image/image.h>

#include "widgets/root.h"

namespace yage::gui
{
	class GuiRenderer
	{
	public:
		GuiRenderer() = delete;

		GuiRenderer(const std::shared_ptr<gl::IContext>& context,
                    const gl::IRenderer::Viewport &viewport);

        /**
         * Renders the complete given UI tree. Hidden widgets are not rendered and hide their respective branch
         * completely.
         * @param root Root of the UI tree.
         */
        void render(RootWidget& root);

	private:
        std::shared_ptr<gl::IRenderer> base_renderer;
		std::shared_ptr<gl::IFrame> frame_buffer;
        std::unique_ptr<gl::IShader> widget_shader;
        std::unique_ptr<gl::IShader> text_shader;

        /**
         * Collects all non-hidden widgets and text objects, sorted by their level in the hierarchy (children after
         * parents, depth-first).
         * @param vector_widget Output vector for widgets.
         * @param vector_text Output vector for text objects.
         * @param widget Parent widget from which to collect.
         */
        void
        collect_drawables(std::vector<std::reference_wrapper<Widget>>& vector_widget,
                          std::vector<std::reference_wrapper<font::Text>>& vector_text,
                          const Widget& widget);
	};
}
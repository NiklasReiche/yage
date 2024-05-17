#include "renderer.h"
#include "shaders.h"
#include <font/shaders.h>

namespace gui
{
	/*************************************************
	** Basic Gui Renderer
	*************************************************/

	GuiRenderer::GuiRenderer(const std::shared_ptr<gl::IRenderer> &base_renderer,
                             const std::shared_ptr<gl::IShaderCreator> &shader_creator, const gl::IRenderer::Viewport &viewport)
		: base_renderer(base_renderer)
	{
		base_renderer->setViewport(viewport);

        widget_shader = shader_creator->createShader(shaders::WidgetShader::vert, shaders::WidgetShader::frag);
        text_shader = shader_creator->createShader(font::shaders::TextShader::vert, font::shaders::TextShader::frag);

		gml::Mat4d projection = gml::matrix::orthographic<float>(0.0, (float)viewport.width, (float)viewport.height, 0.0, 0.1, 100.0);
		widget_shader->setUniform("projection", projection);
		widget_shader->setUniform("texture", 0);
		text_shader->setUniform("projection", projection);
        text_shader->setUniform("scale", 16.0f / 16.0f); // TODO: based on text size

		//clearColor = gml::Vec4<float>(1, 1, 1, 1);
	}
    
    void GuiRenderer::render(RootWidget& root)
    {
        std::vector<std::reference_wrapper<Widget>> widgets;
        std::vector<std::reference_wrapper<font::Text>> texts;
        collect_drawables(widgets, texts, root);

        base_renderer->setClearColor(gl::Color::WHITE);
        base_renderer->clear();
        base_renderer->enableBlending();

        base_renderer->useShader(*widget_shader);
        for (auto widget : widgets) {
            base_renderer->bindTexture(widget.get().texture_atlas_view().get());
            base_renderer->draw(widget.get().drawable());
        }

        base_renderer->useShader(*text_shader);
        for (auto text : texts) {
            base_renderer->bindTexture(text.get().texture(), 0);
            base_renderer->draw(text.get().drawable());
        }

        base_renderer->disableBlending();
    }

    void GuiRenderer::collect_drawables(std::vector<std::reference_wrapper<Widget>>& vector_widget,
                                        std::vector<std::reference_wrapper<font::Text>>& vector_text,
                                        const Widget& widget)
    {
        for (auto& child : widget.children()) {
            if (!child->is_Active()) {
                // stops processing this whole branch
                continue;
            }

            vector_widget.emplace_back(*child);
            if (child->text() != nullptr) {
                vector_text.emplace_back(*child->text());
            }
            collect_drawables(vector_widget, vector_text, *child);
        }
    }
}

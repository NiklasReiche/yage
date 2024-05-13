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
    
    void GuiRenderer::render(RootWidget& root)
    {
        std::vector<Widget*> widgets;
        std::vector<font::Text*> texts;
        collect_drawables(widgets, texts, root);

        glRenderer->setClearColor(gl::Color::WHITE);
        glRenderer->clear();
        glRenderer->enableBlending();

        glRenderer->useShader(*guiShader);
        for (auto widget : widgets) {
            glRenderer->bindTexture(widget->texture_atlas_view().get());
            glRenderer->draw(widget->drawable());
        }

        glRenderer->useShader(*textShader);
        for (auto text : texts) {
            glRenderer->bindTexture(text->texture(), 0);
            glRenderer->draw(text->drawable());
        }

        glRenderer->disableBlending();
    }

    void GuiRenderer::collect_drawables(std::vector<Widget*>& vector_widget,
                                        std::vector<font::Text*>& vector_text,
                                        Widget& widget)
    {
        for (auto& child : widget.children()) {
            if (!child->is_Active()) {
                // stops processing this whole branch
                continue;
            }

            vector_widget.push_back(child.get());
            if (child->text() != nullptr) {
                vector_text.push_back(child->text());
            }
            collect_drawables(vector_widget, vector_text, *child);
        }
    }
}

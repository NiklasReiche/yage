#include "Renderer.h"
#include "Context.h"
#include "RenderTarget.h"

namespace yage::gl::opengl4
{
    Renderer::Renderer(Context* context)
        : m_context(context)
    {
    }

    void Renderer::begin_command_buffer()
    {
        // TODO: emulate command buffer
    }

    void Renderer::begin_render_pass()
    {
        m_context->bind_frame_buffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::begin_render_pass(const IRenderTarget& render_target)
    {
        m_context->bind_frame_buffer(GL_FRAMEBUFFER, static_cast<const RenderTarget&>(render_target).gl_draw_handle());
    }

    void Renderer::bind_descriptor_set(const IDescriptorSet&)
    {
        // TODO
    }

    void Renderer::bind_pipeline(IPipeline& pipeline)
    {
        auto& pipeline_impl = static_cast<Pipeline&>(pipeline);

        m_current_pipeline_primitive = pipeline_impl.primitive();
        pipeline_impl.bind_state();
    }

    void Renderer::draw(const IDrawable2& drawable)
    {
        auto& drawable_impl = static_cast<const Drawable&>(drawable);

        m_context->bind_vao(drawable_impl.gl_vao_handle());
        glDrawElements(m_current_pipeline_primitive, drawable_impl.indices_count(), drawable_impl.indices_count(), nullptr);
    }

    void Renderer::end_render_pass()
    {
        // TODO: emulate command buffer
    }

    void Renderer::end_command_buffer()
    {
        // TODO: emulate command buffer
    }
}

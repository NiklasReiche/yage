#pragma once

#include <glad/gl.h>

#include "../IRenderer.h"

namespace yage::gl::opengl4
{
    class Context;

    class Renderer final : public IRenderer2
    {
    public:
        explicit Renderer(Context* context);

        void begin_command_buffer() override;

        void begin_render_pass() override;

        void begin_render_pass(const IRenderTarget& render_target) override;

        void bind_descriptor_set(const IDescriptorSet& descriptor_set) override;

        void bind_pipeline(IPipeline& pipeline) override;

        void draw(const IDrawable2& drawable) override;

        void end_render_pass() override;

        void end_command_buffer() override;

    private:
        Context* m_context;

        GLenum m_current_pipeline_primitive;
    };
}

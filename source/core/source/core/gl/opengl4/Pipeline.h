#pragma once

#include <glad/gl.h>

#include "../IPipeline.h"

namespace yage::gl::opengl4
{
    class Context;

    struct PipelineDescriptor
    {
        std::vector<GLuint> shader_modules{};
        PrimitiveTopology primitive = PrimitiveTopology::TRIANGLE_LIST;
        // TODO: descriptor layouts
        PolygonMode polygon_mode = PolygonMode::FILL;
        CullMode cull_mode = CullMode::NONE;
        float line_width = 1.0f;
        std::optional<Viewport> viewport{};
        std::optional<ScissorRectangle> scissor{};
        bool with_depth_test = false;
        std::optional<BlendInfo> blending{};
    };

    class Pipeline final : public IPipeline
    {
    public:
        Pipeline(Context* context, const PipelineDescriptor& descriptor);

        ~Pipeline() override;

        Pipeline(const Pipeline& other) = delete;

        Pipeline(Pipeline&& other) noexcept;

        Pipeline& operator=(const Pipeline& other) = delete;

        Pipeline& operator=(Pipeline&& other) noexcept;

        void set_dynamic_viewport(Viewport viewport, ScissorRectangle scissor) override;

        void bind_state();

    private:
        Context* m_context;

        GLuint m_shader_program;

        GLenum m_primitive;
        GLenum m_polygon_mode;
        GLenum m_cull_mode;
        float m_line_width;
        std::optional<Viewport> m_viewport{};
        std::optional<ScissorRectangle> m_scissor{};
        bool m_depth_test = false;

        struct BlendState
        {
            GLenum source_factor;
            GLenum destination_factor;
            GLenum operation;
            math::Vec4f constant;
        };
        std::optional<BlendState> m_blend_state{};

        void clear();

        static void check_shader_link_error(GLuint shader_program);
    };
}

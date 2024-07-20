#pragma once

#include <vector>

#include <glad/gl.h>

#include "../IPipelineBuilder.h"
#include "Pipeline.h"

namespace yage::gl::opengl4
{
    class Context;

    class PipelineBuilder final : public IPipelineBuilder
    {
    public:
        explicit PipelineBuilder(Context* context);

        IPipelineBuilder& with_shaders(ShaderSource vertex_code, ShaderSource fragment_code) override;

        IPipelineBuilder& with_shaders(ShaderSource vertex_code, ShaderSource fragment_code,
                                       ShaderSource geometry_code) override;

        IPipelineBuilder& with_vertex_format(PrimitiveTopology topology, VertexDataInfo vertex_data_info) override;

        IPipelineBuilder& append_layout(DescriptorSetLayoutHandle layout) override;

        IPipelineBuilder& with_rasterizer(PolygonMode polygon_mode, CullMode cull_mode, float line_width) override;

        IPipelineBuilder& with_viewport(Viewport viewport, ScissorRectangle scissor) override;

        IPipelineBuilder& with_dynamic_viewport() override;

        IPipelineBuilder& for_render_target(const IRenderTarget& render_target) override;

        IPipelineBuilder& for_swap_chain_render_target() override;

        IPipelineBuilder& with_depth_test() override;

        IPipelineBuilder& with_blending(BlendInfo blend_info) override;

        PipelineHandle build() override;

        void clear() override;

    private:
        Context* m_context; // TODO

        PipelineDescriptor m_descriptor;

        void destroy_shader_modules();

        static void check_shader_model_compilation_error(const GLuint module);
    };
}

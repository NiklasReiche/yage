#pragma once

#include <string_view>
#include <variant>

#include "IDescriptorSetLayout.h"
#include "IPipeline.h"
#include "IRenderTarget.h"
#include "enums.h"

namespace yage::gl
{
    using ShaderSource = std::variant<std::string_view, std::span<const std::byte>>;

    class IPipelineBuilder
    {
    public:
        virtual ~IPipelineBuilder() = default;

        virtual IPipelineBuilder& with_shaders(ShaderSource vertex_code, ShaderSource fragment_code) = 0;

        virtual IPipelineBuilder& with_shaders(ShaderSource vertex_code, ShaderSource fragment_code,
                                               ShaderSource geometry_code) = 0;

        virtual IPipelineBuilder& with_vertex_format(PrimitiveTopology topology, VertexDataInfo vertex_data_info) = 0;

        virtual IPipelineBuilder& append_layout(DescriptorSetLayoutHandle layout) = 0;

        virtual IPipelineBuilder& with_rasterizer(PolygonMode polygon_mode, CullMode cull_mode, float line_width) = 0;

        virtual IPipelineBuilder& with_viewport(Viewport viewport, ScissorRectangle scissor) = 0;

        virtual IPipelineBuilder& with_render_target(const IRenderTarget& render_target) = 0;

        virtual IPipelineBuilder& with_swap_chain_render_target() = 0;

        // optional
        virtual IPipelineBuilder& with_depth_test() = 0;

        // optional
        virtual IPipelineBuilder& with_blending(BlendInfo blend_info) = 0;

        virtual PipelineHandle build() = 0;

        virtual void clear() = 0;
    };
}

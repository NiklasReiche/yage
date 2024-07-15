#pragma once

#include <span>

#include <vulkan/vulkan.h>

#include "../enums.h"
#include "DescriptorSetLayout.h"
#include "Pipeline.h"
#include "RenderTarget.h"
#include "core/gl/IPipelineBuilder.h"

namespace yage::gl::vulkan
{
    class Instance;

    class PipelineBuilder final : public IPipelineBuilder
    {
    public:
        explicit PipelineBuilder(std::weak_ptr<Instance> instance);

        ~PipelineBuilder() override;

        PipelineBuilder(PipelineBuilder& other) = delete;

        PipelineBuilder(PipelineBuilder&& other) = delete;

        PipelineBuilder& operator=(PipelineBuilder& other) = delete;

        PipelineBuilder& operator=(PipelineBuilder&& other) = delete;

        IPipelineBuilder& with_shaders(ShaderSource vertex_code, ShaderSource fragment_code) override;

        IPipelineBuilder& with_shaders(ShaderSource vertex_code, ShaderSource fragment_code,
                                       ShaderSource geometry_code) override;

        IPipelineBuilder& with_vertex_format(PrimitiveTopology topology, VertexDataInfo vertex_data_info) override;

        IPipelineBuilder& append_layout(DescriptorSetLayoutHandle layout) override;

        IPipelineBuilder& with_rasterizer(PolygonMode polygon_mode, CullMode cull_mode, float line_width) override;

        IPipelineBuilder& with_viewport(Viewport viewport, ScissorRectangle scissor) override;

        IPipelineBuilder& with_render_target(const IRenderTarget& render_target) override;

        IPipelineBuilder& with_swap_chain_render_target() override;

        IPipelineBuilder& with_depth_test() override;

        IPipelineBuilder& with_blending(BlendInfo blend_info) override;

        PipelineHandle build() override;

        void clear() override;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance

        std::vector<VkShaderModule> m_shader_modules{};
        std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages{};

        std::vector<VkVertexInputAttributeDescription> m_vertex_attribute_descriptions{};
        std::vector<VkVertexInputBindingDescription> m_vertex_binding_descriptions{};
        VkPipelineVertexInputStateCreateInfo m_vertex_input_info{};
        VkPipelineInputAssemblyStateCreateInfo m_input_assembly{};

        std::vector<DescriptorSetLayoutHandle> m_descriptor_set_layouts;

        VkPipelineRasterizationStateCreateInfo m_rasterizer_info{};

        VkViewport m_viewport{};
        VkRect2D m_scissor{};
        VkPipelineViewportStateCreateInfo m_viewport_state{};

        VkPipelineRenderingCreateInfo m_rendering_create_info{};
        VkPipelineMultisampleStateCreateInfo m_multisampling_info{};

        VkPipelineDepthStencilStateCreateInfo m_depth_stencil_state_create_info{};

        VkPipelineColorBlendAttachmentState m_blend_attachment{};
        VkPipelineColorBlendStateCreateInfo m_blend_info{};

        std::vector<VkDynamicState> m_dynamic_states{};

        VkShaderModule create_shader_module(std::span<const std::byte> code);

        void destroy_shader_modules();
    };
}

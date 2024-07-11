#pragma once

#include <span>

#include <vulkan/vulkan.h>

#include "../DrawableCreator.h"
#include "../enums.h"
#include "DescriptorSetLayout.h"
#include "FrameBuffer.h"
#include "Pipeline.h"

namespace yage::gl::vulkan
{
    class Instance;

    class PipelineBuilder final
    {
    public:
        explicit PipelineBuilder(std::weak_ptr<Instance> instance);

        ~PipelineBuilder();

        PipelineBuilder(PipelineBuilder& other) = delete;

        PipelineBuilder(PipelineBuilder&& other) = delete;

        PipelineBuilder& operator=(PipelineBuilder& other) = delete;

        PipelineBuilder& operator=(PipelineBuilder&& other) = delete;

        PipelineBuilder& with_shaders(std::span<const std::byte> vertex_code, std::span<const std::byte> fragment_code);

        PipelineBuilder& with_rasterizer(PolygonMode polygon_mode, CullMode cull_mode, float line_width);

        PipelineBuilder& with_vertex_format(PrimitiveTopology topology, std::span<const VertexComponent> layout, VertexFormat format);

        PipelineBuilder& with_viewport(Viewport viewport, ScissorRectangle scissor);

        PipelineBuilder& with_render_pass(std::shared_ptr<RenderPassHandle> render_pass);

        PipelineBuilder& with_layout(DescriptorSetLayoutHandle layout);

        PipelineHandle build();

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance

        std::array<VkShaderModule, 2> m_shader_modules{};
        std::array<VkPipelineShaderStageCreateInfo, 2> m_shader_stages{};

        VkPipelineInputAssemblyStateCreateInfo m_input_assembly{};
        std::vector<VkVertexInputBindingDescription> m_vertex_binding_descriptions{};
        std::vector<VkVertexInputAttributeDescription> m_vertex_attribute_descriptions{};
        VkPipelineVertexInputStateCreateInfo m_vertex_input_info{};

        VkViewport m_viewport{};
        VkRect2D m_scissor{};
        VkPipelineViewportStateCreateInfo m_viewport_state{};

        std::vector<VkDynamicState> m_dynamic_states{};
        VkPipelineDynamicStateCreateInfo m_dynamic_state{};

        std::shared_ptr<RenderPassHandle> m_render_pass{};

        VkPipelineRasterizationStateCreateInfo m_rasterizer_info{};
        VkPipelineColorBlendAttachmentState m_blend_attachment{};
        VkPipelineColorBlendStateCreateInfo m_blend_info{};
        VkPipelineMultisampleStateCreateInfo m_multisampling_info{};
        VkPipelineLayoutCreateInfo m_pipeline_layout_info{};

        PipelineBuilder& with_blending();

        PipelineBuilder& with_multisampling();

        PipelineBuilder& with_dynamic_state();

        VkShaderModule create_shader_module(std::span<const std::byte> code);

        void destroy_shader_modules();
    };
}

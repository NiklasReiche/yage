#include "PipelineBuilder.h"
#include "Instance.h"
#include <utility>

#include "enums.h"

namespace yage::gl::vulkan
{
    PipelineBuilder::PipelineBuilder(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    PipelineBuilder::~PipelineBuilder()
    {
        destroy_shader_modules();
    }

    PipelineBuilder& PipelineBuilder::with_shaders(const std::span<const std::byte> vertex_code,
                                                   const std::span<const std::byte> fragment_code)
    {
        destroy_shader_modules();

        const VkShaderModule vert_module = create_shader_module(vertex_code);
        VkPipelineShaderStageCreateInfo vert_stage_info{};
        vert_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_stage_info.module = vert_module;
        vert_stage_info.pName = "main";
        vert_stage_info.pSpecializationInfo = nullptr; // injection of constants

        const VkShaderModule frag_module = create_shader_module(fragment_code);
        VkPipelineShaderStageCreateInfo frag_stage_info{};
        frag_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_stage_info.module = frag_module;
        frag_stage_info.pName = "main";
        vert_stage_info.pSpecializationInfo = nullptr; // injection of constants

        m_shader_modules = {vert_module, frag_module};
        m_shader_stages = {vert_stage_info, frag_stage_info};

        return *this;
    }

    PipelineBuilder& PipelineBuilder::with_rasterizer(const PolygonMode polygon_mode, const CullMode cull_mode,
                                                      const float line_width)
    {
        m_rasterizer_info = VkPipelineRasterizationStateCreateInfo{};
        m_rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

        m_rasterizer_info.depthClampEnable = VK_FALSE;
        m_rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
        m_rasterizer_info.polygonMode = convert(polygon_mode);
        m_rasterizer_info.lineWidth = line_width;
        m_rasterizer_info.cullMode = convert(cull_mode);
        m_rasterizer_info.frontFace = VK_FRONT_FACE_CLOCKWISE;

        m_rasterizer_info.depthBiasEnable = VK_FALSE;
        m_rasterizer_info.depthBiasConstantFactor = 0.0f; // Optional
        m_rasterizer_info.depthBiasClamp = 0.0f; // Optional
        m_rasterizer_info.depthBiasSlopeFactor = 0.0f; // Optional

        return *this;
    }


    PipelineBuilder& PipelineBuilder::with_vertex_format(const PrimitiveTopology topology,
                                                         const std::span<const VertexComponent> layout,
                                                         const VertexFormat format)
    {
        m_vertex_binding_descriptions.clear();
        m_vertex_attribute_descriptions.clear();

        m_input_assembly = VkPipelineInputAssemblyStateCreateInfo{};
        m_input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_input_assembly.topology = convert(topology);
        m_input_assembly.primitiveRestartEnable = VK_FALSE;

        if (layout.empty()) {
            m_vertex_input_info = VkPipelineVertexInputStateCreateInfo{};
            m_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            m_vertex_input_info.vertexBindingDescriptionCount = 0;
            m_vertex_input_info.pVertexBindingDescriptions = nullptr;
            m_vertex_input_info.vertexAttributeDescriptionCount = 0;
            m_vertex_input_info.pVertexAttributeDescriptions = nullptr;
            return *this;
        }

        std::size_t vertex_size_bytes = 0;
        for (const VertexComponent& component: layout) {
            vertex_size_bytes += byte_size(component);
        }

        switch (format) {
            case VertexFormat::INTERLEAVED: {
                // for interleaved components we can reuse the same binding description
                VkVertexInputBindingDescription binding_description{};
                binding_description.binding = 0;
                binding_description.stride = vertex_size_bytes;
                binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // non-instanced rendering
                m_vertex_binding_descriptions.push_back(binding_description);

                m_vertex_attribute_descriptions.reserve(layout.size());
                std::size_t offset = 0;
                for (std::size_t i = 0; i < layout.size(); ++i) {
                    const VertexComponent& component = layout[i];
                    VkVertexInputAttributeDescription attribute_description{};
                    attribute_description.binding = 0;
                    attribute_description.location = i;
                    attribute_description.format = convert(component);
                    attribute_description.offset = offset;

                    m_vertex_attribute_descriptions.push_back(attribute_description);
                    offset += byte_size(component);
                }

                break;
            }
            // TODO: batched
            default:
                throw std::invalid_argument("Unknown VertexFormat value");
        }

        m_vertex_input_info = VkPipelineVertexInputStateCreateInfo{};
        m_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_vertex_input_info.vertexBindingDescriptionCount = m_vertex_binding_descriptions.size();
        m_vertex_input_info.pVertexBindingDescriptions = m_vertex_binding_descriptions.data();
        m_vertex_input_info.vertexAttributeDescriptionCount = m_vertex_attribute_descriptions.size();
        m_vertex_input_info.pVertexAttributeDescriptions = m_vertex_attribute_descriptions.data();

        return *this;
    }

    PipelineBuilder& PipelineBuilder::with_viewport(const Viewport viewport, const ScissorRectangle scissor)
    {
        m_viewport = VkViewport{};
        m_viewport.x = viewport.x;
        m_viewport.y = viewport.y;
        m_viewport.width = viewport.width;
        m_viewport.height = viewport.height;
        m_viewport.minDepth = 0.0f;
        m_viewport.maxDepth = 1.0f;

        m_scissor.offset = {scissor.x, scissor.y};
        m_scissor.extent = {scissor.width, scissor.height};

        m_viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        m_viewport_state.viewportCount = 1;
        m_viewport_state.pViewports = &m_viewport;
        m_viewport_state.scissorCount = 1;
        m_viewport_state.pScissors = &m_scissor;

        return *this;
    }

    PipelineBuilder& PipelineBuilder::with_render_pass(std::shared_ptr<RenderPassHandle> render_pass)
    {
        m_render_pass = std::move(render_pass);

        return *this;
    }

    PipelineHandle PipelineBuilder::build()
    {
        with_blending();
        with_multisampling();
        with_dynamic_state();

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = m_shader_stages.size();
        pipeline_info.pStages = m_shader_stages.data();
        pipeline_info.pVertexInputState = &m_vertex_input_info;
        pipeline_info.pInputAssemblyState = &m_input_assembly;
        pipeline_info.pViewportState = &m_viewport_state;
        pipeline_info.pRasterizationState = &m_rasterizer_info;
        pipeline_info.pMultisampleState = &m_multisampling_info;
        pipeline_info.pDepthStencilState = nullptr; // Optional
        pipeline_info.pColorBlendState = &m_blend_info;
        pipeline_info.pDynamicState = &m_dynamic_state;
        pipeline_info.renderPass = m_render_pass->get<RenderPass>().vk_handle();
        pipeline_info.subpass = 0;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipeline_info.basePipelineIndex = -1; // Optional

        const auto instance = m_instance.lock();
        return instance->store_pipelines().create(instance.get(), m_render_pass, pipeline_info, m_pipeline_layout_info);
    }

    PipelineBuilder& PipelineBuilder::with_blending()
    {
        m_blend_attachment = VkPipelineColorBlendAttachmentState{};
        m_blend_attachment.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
        m_blend_attachment.blendEnable = VK_FALSE;
        m_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        m_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        m_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        m_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        m_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        m_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        m_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_blend_info.logicOpEnable = VK_FALSE;
        m_blend_info.logicOp = VK_LOGIC_OP_COPY; // Optional
        m_blend_info.attachmentCount = 1;
        m_blend_info.pAttachments = &m_blend_attachment;
        m_blend_info.blendConstants[0] = 0.0f; // Optional
        m_blend_info.blendConstants[1] = 0.0f; // Optional
        m_blend_info.blendConstants[2] = 0.0f; // Optional
        m_blend_info.blendConstants[3] = 0.0f; // Optional

        return *this;
    }

    PipelineBuilder& PipelineBuilder::with_multisampling()
    {
        m_multisampling_info = VkPipelineMultisampleStateCreateInfo{};
        m_multisampling_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m_multisampling_info.sampleShadingEnable = VK_FALSE;
        m_multisampling_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        m_multisampling_info.minSampleShading = 1.0f; // Optional
        m_multisampling_info.pSampleMask = nullptr; // Optional
        m_multisampling_info.alphaToCoverageEnable = VK_FALSE; // Optional
        m_multisampling_info.alphaToOneEnable = VK_FALSE; // Optional

        return *this;
    }

    PipelineBuilder& PipelineBuilder::with_layout(DescriptorSetLayoutHandle layout)
    {
        // TODO: give ownership to pipeline of descriptor layout
        m_pipeline_layout_info = VkPipelineLayoutCreateInfo{};
        m_pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        m_pipeline_layout_info.setLayoutCount = 1;
        m_pipeline_layout_info.pSetLayouts = &layout.get<DescriptorSetLayout>().vk_handle();
        m_pipeline_layout_info.pushConstantRangeCount = 0; // Optional
        m_pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

        return *this;
    }

    PipelineBuilder& PipelineBuilder::with_dynamic_state()
    {
        m_dynamic_states = {};

        m_dynamic_state = VkPipelineDynamicStateCreateInfo{};
        m_dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        m_dynamic_state.dynamicStateCount = m_dynamic_states.size();
        m_dynamic_state.pDynamicStates = m_dynamic_states.data();

        return *this;
    }

    VkShaderModule PipelineBuilder::create_shader_module(const std::span<const std::byte> code)
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shader_module;
        if (vkCreateShaderModule(m_instance.lock()->device(), &create_info, nullptr, &shader_module) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shader_module;
    }

    void PipelineBuilder::destroy_shader_modules()
    {
        const VkDevice device = m_instance.lock()->device();
        for (const VkShaderModule module: m_shader_modules) {
            if (module != nullptr) {
                vkDestroyShaderModule(device, module, nullptr);
            }
        }
    }
}

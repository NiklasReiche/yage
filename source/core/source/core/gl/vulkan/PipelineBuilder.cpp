#include "PipelineBuilder.h"
#include <utility>
#include "Instance.h"

#include "enums.h"

namespace yage::gl::vulkan
{
    PipelineBuilder::PipelineBuilder(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
        clear();
    }

    PipelineBuilder::~PipelineBuilder()
    {
        destroy_shader_modules();
    }

    IPipelineBuilder& PipelineBuilder::with_shaders(const ShaderSource vertex_code, const ShaderSource fragment_code)
    {
        destroy_shader_modules();

        const VkShaderModule vert_module = create_shader_module(std::get<std::span<const std::byte>>(vertex_code));
        VkPipelineShaderStageCreateInfo vert_stage_info{};
        vert_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_stage_info.module = vert_module;
        vert_stage_info.pName = "main";
        vert_stage_info.pSpecializationInfo = nullptr; // injection of constants

        const VkShaderModule frag_module = create_shader_module(std::get<std::span<const std::byte>>(fragment_code));
        VkPipelineShaderStageCreateInfo frag_stage_info{};
        frag_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_stage_info.module = frag_module;
        frag_stage_info.pName = "main";
        frag_stage_info.pSpecializationInfo = nullptr; // injection of constants

        m_shader_modules.push_back(vert_module);
        m_shader_modules.push_back(frag_module);

        m_shader_stages.push_back(vert_stage_info);
        m_shader_stages.push_back(frag_stage_info);

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_shaders(const ShaderSource vertex_code, const ShaderSource fragment_code,
                                                    const ShaderSource geometry_code)
    {
        with_shaders(vertex_code, fragment_code);

        const VkShaderModule geom_module = create_shader_module(std::get<std::span<const std::byte>>(geometry_code));
        VkPipelineShaderStageCreateInfo geom_stage_info{};
        geom_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        geom_stage_info.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
        geom_stage_info.module = geom_module;
        geom_stage_info.pName = "main";
        geom_stage_info.pSpecializationInfo = nullptr; // injection of constants

        m_shader_modules.push_back(geom_module);
        m_shader_stages.push_back(geom_stage_info);

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_vertex_format(const PrimitiveTopology topology,
                                                          const VertexDataInfo vertex_data_info)
    {
        m_vertex_binding_descriptions.clear();
        m_vertex_attribute_descriptions.clear();

        m_input_assembly = VkPipelineInputAssemblyStateCreateInfo{};
        m_input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_input_assembly.topology = convert(topology);
        m_input_assembly.primitiveRestartEnable = VK_FALSE;

        if (vertex_data_info.vertex_description.empty()) {
            m_vertex_input_info = VkPipelineVertexInputStateCreateInfo{};
            m_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            m_vertex_input_info.vertexBindingDescriptionCount = 0;
            m_vertex_input_info.pVertexBindingDescriptions = nullptr;
            m_vertex_input_info.vertexAttributeDescriptionCount = 0;
            m_vertex_input_info.pVertexAttributeDescriptions = nullptr;
            return *this;
        }

        std::size_t vertex_size_bytes = 0;
        for (const VertexComponent& component: vertex_data_info.vertex_description) {
            vertex_size_bytes += byte_size(component);
        }

        switch (vertex_data_info.data_layout) {
            case VertexDataLayout::INTERLEAVED: {
                // for interleaved components we can reuse the same binding description
                VkVertexInputBindingDescription binding_description{};
                binding_description.binding = 0;
                binding_description.stride = vertex_size_bytes;
                binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // non-instanced rendering
                m_vertex_binding_descriptions.push_back(binding_description);

                m_vertex_attribute_descriptions.reserve(vertex_data_info.vertex_description.size());
                std::size_t offset = 0;
                for (std::size_t i = 0; i < vertex_data_info.vertex_description.size(); ++i) {
                    const VertexComponent& component = vertex_data_info.vertex_description[i];
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
            default: throw std::invalid_argument("Unknown VertexFormat value");
        }

        m_vertex_input_info = VkPipelineVertexInputStateCreateInfo{};
        m_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_vertex_input_info.vertexBindingDescriptionCount = m_vertex_binding_descriptions.size();
        m_vertex_input_info.pVertexBindingDescriptions = m_vertex_binding_descriptions.data();
        m_vertex_input_info.vertexAttributeDescriptionCount = m_vertex_attribute_descriptions.size();
        m_vertex_input_info.pVertexAttributeDescriptions = m_vertex_attribute_descriptions.data();

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::append_layout(const DescriptorSetLayoutHandle layout)
    {
        m_descriptor_set_layouts.push_back(layout);

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_rasterizer(const PolygonMode polygon_mode, const CullMode cull_mode,
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

    IPipelineBuilder& PipelineBuilder::with_viewport(const Viewport viewport, const ScissorRectangle scissor)
    {
        m_viewport = VkViewport{};
        m_viewport.x = viewport.x;
        m_viewport.y = viewport.y;
        m_viewport.width = viewport.width;
        m_viewport.height = viewport.height;
        m_viewport.minDepth = 0.0f;
        m_viewport.maxDepth = 1.0f;

        m_scissor = VkRect2D{};
        m_scissor.offset = {scissor.x, scissor.y};
        m_scissor.extent = {scissor.width, scissor.height};

        m_viewport_state = VkPipelineViewportStateCreateInfo{};
        m_viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        m_viewport_state.viewportCount = 1;
        m_viewport_state.pViewports = &m_viewport;
        m_viewport_state.scissorCount = 1;
        m_viewport_state.pScissors = &m_scissor;

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_render_target(const IRenderTarget& render_target)
    {
        const auto& render_target_impl = static_cast<const RenderTarget&>(render_target);

        const std::vector<VkFormat>& color_attachment_formats = render_target_impl.color_attachment_formats();
        const std::optional<VkFormat>& depth_attachment_format = render_target_impl.depth_attachment_format();

        m_rendering_create_info.colorAttachmentCount = color_attachment_formats.size();
        m_rendering_create_info.pColorAttachmentFormats = color_attachment_formats.data();
        if (depth_attachment_format.has_value()) {
            m_rendering_create_info.depthAttachmentFormat = depth_attachment_format.value();
        }

        m_multisampling_info.rasterizationSamples = convert(render_target_impl.samples());

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_swap_chain_render_target()
    {
        const auto& swap_chain = m_instance.lock()->swap_chain();

        m_rendering_create_info = VkPipelineRenderingCreateInfo{};
        m_rendering_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        m_rendering_create_info.colorAttachmentCount = 1;
        m_rendering_create_info.pColorAttachmentFormats = &swap_chain.color_format();
        m_rendering_create_info.depthAttachmentFormat = swap_chain.depth_format();

        m_multisampling_info.rasterizationSamples = swap_chain.samples();

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_depth_test()
    {
        m_depth_stencil_state_create_info.depthTestEnable = VK_TRUE;
        m_depth_stencil_state_create_info.depthWriteEnable = VK_TRUE;
        m_depth_stencil_state_create_info.depthCompareOp = VK_COMPARE_OP_LESS;

        m_depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE;
        m_depth_stencil_state_create_info.minDepthBounds = 0.0f; // Optional
        m_depth_stencil_state_create_info.maxDepthBounds = 1.0f; // Optional

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_blending(BlendInfo blend_info)
    {
        m_blend_attachment = VkPipelineColorBlendAttachmentState{};
        m_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        m_blend_attachment.blendEnable = VK_TRUE;
        m_blend_attachment.srcColorBlendFactor = convert(blend_info.source_factor);
        m_blend_attachment.dstColorBlendFactor = convert(blend_info.destination_factor);
        m_blend_attachment.colorBlendOp = convert(blend_info.operation);
        m_blend_attachment.srcAlphaBlendFactor = convert(blend_info.source_factor);
        m_blend_attachment.dstAlphaBlendFactor = convert(blend_info.destination_factor);
        m_blend_attachment.alphaBlendOp = convert(blend_info.operation);

        m_blend_info = VkPipelineColorBlendStateCreateInfo{};
        m_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_blend_info.logicOpEnable = VK_FALSE;
        m_blend_info.attachmentCount = 1;
        m_blend_info.pAttachments = &m_blend_attachment;
        m_blend_info.blendConstants[0] = blend_info.constant(0);
        m_blend_info.blendConstants[1] = blend_info.constant(1);
        m_blend_info.blendConstants[2] = blend_info.constant(2);
        m_blend_info.blendConstants[3] = blend_info.constant(3);

        return *this;
    }

    PipelineHandle PipelineBuilder::build()
    {
        std::vector<VkDescriptorSetLayout> vk_layouts(m_descriptor_set_layouts.size());
        for (std::size_t i = 0; i < m_descriptor_set_layouts.size(); ++i) {
            vk_layouts[i] = m_descriptor_set_layouts[i].get<DescriptorSetLayout>().vk_handle();
        }

        VkPipelineDynamicStateCreateInfo dynamic_state_info{};
        dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_info.dynamicStateCount = m_dynamic_states.size();
        dynamic_state_info.pDynamicStates = m_dynamic_states.data();

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.pNext = &m_rendering_create_info;
        pipeline_info.stageCount = m_shader_stages.size();
        pipeline_info.pStages = m_shader_stages.data();
        pipeline_info.pVertexInputState = &m_vertex_input_info;
        pipeline_info.pInputAssemblyState = &m_input_assembly;
        pipeline_info.pViewportState = &m_viewport_state;
        pipeline_info.pRasterizationState = &m_rasterizer_info;
        pipeline_info.pMultisampleState = &m_multisampling_info;
        pipeline_info.pDepthStencilState = &m_depth_stencil_state_create_info;
        pipeline_info.pColorBlendState = &m_blend_info;
        pipeline_info.pDynamicState = &dynamic_state_info;
        pipeline_info.renderPass = nullptr; // uses dynamic rendering
        pipeline_info.subpass = 0;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipeline_info.basePipelineIndex = -1; // Optional

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = vk_layouts.size();
        pipeline_layout_info.pSetLayouts = vk_layouts.data();
        pipeline_layout_info.pushConstantRangeCount = 0; // Optional
        pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

        const auto instance = m_instance.lock();
        return instance->store_pipelines().create(instance.get(), pipeline_info, pipeline_layout_info, m_descriptor_set_layouts);
    }

    void PipelineBuilder::clear()
    {
        // shaders
        destroy_shader_modules();
        m_shader_stages.clear();

        // input assembly
        m_vertex_attribute_descriptions.clear();
        m_vertex_binding_descriptions.clear();
        m_vertex_input_info = VkPipelineVertexInputStateCreateInfo{};
        m_vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_input_assembly = VkPipelineInputAssemblyStateCreateInfo{};
        m_input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

        // descriptor layouts
        m_descriptor_set_layouts.clear();

        // rasterizer
        m_rasterizer_info = VkPipelineRasterizationStateCreateInfo{};
        m_rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

        // viewport
        m_viewport = VkViewport{};
        m_scissor = VkRect2D{};
        m_viewport_state = VkPipelineViewportStateCreateInfo{};
        m_viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

        // render target
        m_rendering_create_info = VkPipelineRenderingCreateInfo{};
        m_rendering_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        m_rendering_create_info.colorAttachmentCount = 0;
        m_rendering_create_info.pColorAttachmentFormats = VK_NULL_HANDLE;
        m_rendering_create_info.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
        m_rendering_create_info.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

        m_multisampling_info = VkPipelineMultisampleStateCreateInfo{};
        m_multisampling_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m_multisampling_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // MSAA
        m_multisampling_info.sampleShadingEnable = VK_FALSE; // SSAA
        m_multisampling_info.minSampleShading = 1.0f; // Optional
        m_multisampling_info.pSampleMask = VK_NULL_HANDLE; // Optional
        m_multisampling_info.alphaToCoverageEnable = VK_FALSE; // Optional
        m_multisampling_info.alphaToOneEnable = VK_FALSE; // Optional

        // depth and stencil tests
        m_depth_stencil_state_create_info = VkPipelineDepthStencilStateCreateInfo{};
        m_depth_stencil_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        m_depth_stencil_state_create_info.depthTestEnable = VK_FALSE;
        m_depth_stencil_state_create_info.depthWriteEnable = VK_FALSE;
        m_depth_stencil_state_create_info.stencilTestEnable = VK_FALSE;

        // blending
        m_blend_attachment = VkPipelineColorBlendAttachmentState{};
        m_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        m_blend_attachment.blendEnable = VK_FALSE;

        m_blend_info = VkPipelineColorBlendStateCreateInfo{};
        m_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_blend_info.logicOpEnable = VK_FALSE;
        m_blend_info.attachmentCount = 1;
        m_blend_info.pAttachments = &m_blend_attachment;

        // dynamic state
        m_dynamic_states.clear();
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

        m_shader_modules.clear();
    }
}

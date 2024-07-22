#include "Pipeline.h"
#include <stdexcept>
#include <utility>
#include "Instance.h"

namespace yage::gl::vulkan
{
    Pipeline::Pipeline(Instance* instance, VkGraphicsPipelineCreateInfo& pipeline_info,
                       const VkPipelineLayoutCreateInfo& layout_info,
                       std::span<DescriptorSetLayoutHandle> descriptor_set_layouts,
                       std::span<VkDynamicState> dynamic_state)
        : m_instance(instance),
          m_vk_device(m_instance->device())
    {
        m_descriptor_set_layouts.reserve(descriptor_set_layouts.size());
        m_descriptor_set_layouts.insert(m_descriptor_set_layouts.end(), descriptor_set_layouts.begin(),
                                        descriptor_set_layouts.end());

        if (vkCreatePipelineLayout(m_vk_device, &layout_info, nullptr, &m_graphics_pipeline_layout) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create pipeline layout!");
        }

        pipeline_info.layout = m_graphics_pipeline_layout;

        if (vkCreateGraphicsPipelines(m_vk_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_vk_handle) !=
            VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create graphics pipeline!");
        }

        if (std::ranges::find(dynamic_state, VK_DYNAMIC_STATE_VIEWPORT) != dynamic_state.end()) {
            set_dynamic_viewport(Viewport{}, ScissorRectangle{});
        }
    }

    Pipeline::~Pipeline()
    {
        clear();
    }

    Pipeline::Pipeline(Pipeline&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_vk_handle(other.m_vk_handle),
          m_graphics_pipeline_layout(other.m_graphics_pipeline_layout)
    {
        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
        other.m_graphics_pipeline_layout = VK_NULL_HANDLE;
    }

    Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_vk_handle = other.m_vk_handle;
        m_graphics_pipeline_layout = other.m_graphics_pipeline_layout;

        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
        other.m_graphics_pipeline_layout = VK_NULL_HANDLE;

        return *this;
    }

    void Pipeline::set_dynamic_viewport(const Viewport viewport, const ScissorRectangle scissor)
    {
        m_dynamic_viewport = VkViewport{
                .x = viewport.x,
                .y = viewport.y,
                .width = viewport.width,
                .height = viewport.height,
                .minDepth = 0.0f,
                .maxDepth = 1.0f,
        };

        m_dynamic_scissor = VkRect2D{.offset = {scissor.x, scissor.y}, .extent = {scissor.width, scissor.height}};
    }

    VkPipeline Pipeline::vk_handle() const
    {
        return m_vk_handle;
    }

    VkPipelineLayout Pipeline::vk_layout() const
    {
        return m_graphics_pipeline_layout;
    }

    void Pipeline::submit_dynamic_state(const VkCommandBuffer command_buffer)
    {
        // TODO: this would also submit for a static pipeline on which set_dynamic_viewport was called
        if (m_dynamic_viewport.has_value()) {
            vkCmdSetViewport(command_buffer, 0, 1, &m_dynamic_viewport.value());
        }
        if (m_dynamic_scissor.has_value()) {
            vkCmdSetScissor(command_buffer, 0, 1, &m_dynamic_scissor.value());
        }
    }

    void Pipeline::clear()
    {
        if (m_vk_handle != VK_NULL_HANDLE) {
            vkDestroyPipeline(m_vk_device, m_vk_handle, nullptr);
        }
        if (m_graphics_pipeline_layout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(m_vk_device, m_graphics_pipeline_layout, nullptr);
        }
    }
}

#include "Pipeline.h"
#include <stdexcept>
#include "Instance.h"

namespace yage::gl::vulkan
{
    Pipeline::Pipeline(Instance* instance, std::shared_ptr<RenderPassHandle> render_pass,
                       VkGraphicsPipelineCreateInfo& pipeline_info, const VkPipelineLayoutCreateInfo& layout_info)
        : m_instance(instance),
          m_vk_device(m_instance->device()),
          m_render_pass(std::move(render_pass))
    {
        if (vkCreatePipelineLayout(m_vk_device, &layout_info, nullptr, &m_graphics_pipeline_layout) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create pipeline layout!");
        }

        pipeline_info.layout = m_graphics_pipeline_layout;

        if (vkCreateGraphicsPipelines(m_vk_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_vk_handle) !=
            VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create graphics pipeline!");
        }
    }

    Pipeline::~Pipeline()
    {
        if (m_vk_handle != VK_NULL_HANDLE) {
            vkDestroyPipeline(m_vk_device, m_vk_handle, nullptr);
            vkDestroyPipelineLayout(m_vk_device, m_graphics_pipeline_layout, nullptr);
        }
    }

    Pipeline::Pipeline(Pipeline&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_render_pass(std::move(other.m_render_pass)),
          m_vk_handle(other.m_vk_handle),
          m_graphics_pipeline_layout(other.m_graphics_pipeline_layout)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
        other.m_graphics_pipeline_layout = VK_NULL_HANDLE;
    }

    Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device,
        m_render_pass = std::move(other.m_render_pass);
        m_vk_handle = other.m_vk_handle;
        m_graphics_pipeline_layout = other.m_graphics_pipeline_layout;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
        other.m_graphics_pipeline_layout = VK_NULL_HANDLE;

        return *this;
    }

    VkPipeline Pipeline::vk_handle() const
    {
        return m_vk_handle;
    }

    VkPipelineLayout Pipeline::vk_layout() const
    {
        return m_graphics_pipeline_layout;
    }
}

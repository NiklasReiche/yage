#include "Pipeline.h"
#include "Instance.h"
#include <stdexcept>

namespace yage::gl::vulkan
{
    Pipeline::Pipeline(std::weak_ptr<Instance> instance, std::shared_ptr<Handle<RenderPass>> render_pass,
                       VkGraphicsPipelineCreateInfo& pipeline_info,
                       const VkPipelineLayoutCreateInfo& layout_info)
        : m_instance(std::move(instance)),
          m_render_pass(std::move(render_pass))
    {
        VkDevice device = m_instance.lock()->device();

        if (vkCreatePipelineLayout(device, &layout_info, nullptr, &m_graphics_pipeline_layout) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create pipeline layout!");
        }

        pipeline_info.layout = m_graphics_pipeline_layout;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr,
                                      &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create graphics pipeline!");
        }
    }

    Pipeline::~Pipeline()
    {
        VkDevice device = m_instance.lock()->device();
        vkDestroyPipeline(device, m_vk_handle, nullptr);
        vkDestroyPipelineLayout(device, m_graphics_pipeline_layout, nullptr);
    }

    VkPipeline Pipeline::vk_handle() const
    {
        return m_vk_handle;
    }
}

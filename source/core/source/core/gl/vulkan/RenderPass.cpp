#include "RenderPass.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    RenderPass::RenderPass(std::weak_ptr<Instance> instance, const VkRenderPassCreateInfo& create_info)
        : m_instance(std::move(instance))
    {
        if (vkCreateRenderPass(m_instance.lock()->device(), &create_info, nullptr, &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create render pass!");
        }
    }

    RenderPass::~RenderPass()
    {
        vkDestroyRenderPass(m_instance.lock()->device(), m_vk_handle, nullptr);
    }

    VkRenderPass RenderPass::vk_handle() const
    {
        return m_vk_handle;
    }
}

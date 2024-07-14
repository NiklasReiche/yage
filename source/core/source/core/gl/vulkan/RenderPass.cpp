#include "RenderPass.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    RenderPass::RenderPass(Instance* instance, const VkRenderPassCreateInfo& create_info, const VkSampleCountFlagBits samples)
        : m_instance(instance),
          m_vk_device(m_instance->device()),
          m_msaa_samples(samples)
    {
        if (vkCreateRenderPass(m_vk_device, &create_info, nullptr, &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create render pass!");
        }
    }

    RenderPass::~RenderPass()
    {
        if (m_vk_handle != VK_NULL_HANDLE) {
            vkDestroyRenderPass(m_vk_device, m_vk_handle, nullptr);
        }
    }

    RenderPass::RenderPass(RenderPass&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_vk_handle(other.m_vk_handle),
          m_msaa_samples(other.m_msaa_samples)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
    }

    RenderPass& RenderPass::operator=(RenderPass&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_vk_handle = other.m_vk_handle;
        m_msaa_samples = other.m_msaa_samples;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;

        return *this;
    }

    VkRenderPass RenderPass::vk_handle() const
    {
        return m_vk_handle;
    }

    VkSampleCountFlagBits RenderPass::samples()
    {
        return m_msaa_samples;
    }
}

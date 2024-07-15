#include "DescriptorSetLayout.h"

#include "Instance.h"

namespace yage::gl::vulkan
{
    DescriptorSetLayout::DescriptorSetLayout(Instance* instance, const VkDescriptorSetLayoutCreateInfo& create_info)
        : m_instance(instance),
          m_vk_device(m_instance->device())
    {
        if (vkCreateDescriptorSetLayout(m_vk_device, &create_info, nullptr, &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create descriptor set layout!");
        }
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        clear();
    }

    DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_vk_handle(other.m_vk_handle)
    {
        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
    }

    DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_vk_handle = other.m_vk_handle;

        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;

        return *this;
    }

    const VkDescriptorSetLayout& DescriptorSetLayout::vk_handle() const
    {
        return m_vk_handle;
    }

    void DescriptorSetLayout::clear()
    {
        if (m_vk_handle != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(m_vk_device, m_vk_handle, nullptr);
        }
    }
}

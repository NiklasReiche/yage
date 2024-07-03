#include "ImageView.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    ImageView::ImageView(std::weak_ptr<Instance> instance, const VkImageViewCreateInfo& create_info)
        : m_instance(std::move(instance)),
          m_vk_device(m_instance.lock()->device())
    {
        if (vkCreateImageView(m_vk_device, &create_info, nullptr, &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create image views!");
        }
    }

    ImageView::~ImageView()
    {
        if (m_vk_handle != VK_NULL_HANDLE) {
            vkDestroyImageView(m_vk_device, m_vk_handle, nullptr);
        }
    }

    ImageView::ImageView(ImageView&& other) noexcept
        : m_instance(std::move(other.m_instance)),
          m_vk_device(other.m_vk_device),
          m_vk_handle(other.m_vk_handle)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
    }

    ImageView& ImageView::operator=(ImageView&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = std::move(other.m_instance);
        m_vk_device = other.m_vk_device;
        m_vk_handle = other.m_vk_handle;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;

        return *this;
    }

    VkImageView ImageView::vk_handle() const
    {
        return m_vk_handle;
    }
}

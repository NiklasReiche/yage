#include "ImageView.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    ImageView::ImageView(std::weak_ptr<Instance> instance, const VkImageViewCreateInfo& create_info)
        : m_instance(std::move(instance))
    {
        if (vkCreateImageView(m_instance.lock()->device(), &create_info, nullptr, &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create image views!");
        }
    }

    ImageView::~ImageView()
    {
        vkDestroyImageView(m_instance.lock()->device(), m_vk_handle, nullptr);
    }

    VkImageView ImageView::vk_handle() const
    {
        return m_vk_handle;
    }
}

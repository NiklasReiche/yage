#include "ImageView.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    ImageView::ImageView(std::weak_ptr<Instance> instance, const std::span<VkImageViewCreateInfo> create_info,
                         const FrameCounter frame_counter)
        : m_instance(std::move(instance)),
          m_vk_device(m_instance.lock()->device()),
          m_frame_counter(frame_counter)
    {
        const std::size_t n_instances = m_frame_counter.max_frame_index;

        assert(create_info.size() == n_instances);

        m_vk_handles.resize(n_instances);
        for (std::size_t i = 0; i < n_instances; ++i) {
            if (vkCreateImageView(m_vk_device, &create_info[i], nullptr, &m_vk_handles[i]) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan: failed to create image views!");
            }
        }
    }

    ImageView::~ImageView()
    {
        for (const VkImageView handle: m_vk_handles) {
            if (handle != VK_NULL_HANDLE) {
                vkDestroyImageView(m_vk_device, handle, nullptr);
            }
        }
    }

    ImageView::ImageView(ImageView&& other) noexcept
        : m_instance(std::move(other.m_instance)),
          m_vk_device(other.m_vk_device),
          m_vk_handles(std::move(other.m_vk_handles)),
          m_frame_counter(other.m_frame_counter)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        for (VkImageView& m_vk_handle: other.m_vk_handles) {
            m_vk_handle = VK_NULL_HANDLE;
        }
    }

    ImageView& ImageView::operator=(ImageView&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = std::move(other.m_instance);
        m_vk_device = other.m_vk_device;
        m_vk_handles = other.m_vk_handles;
        m_frame_counter = other.m_frame_counter;

        other.m_vk_device = VK_NULL_HANDLE;
        for (VkImageView& m_vk_handle: other.m_vk_handles) {
            m_vk_handle = VK_NULL_HANDLE;
        }

        return *this;
    }

    VkImageView ImageView::vk_handle() const
    {
        return m_vk_handles[*m_frame_counter.curent_frame_index];
    }

    VkImageView ImageView::vk_handle(const std::size_t index) const
    {
        return m_vk_handles[index];
    }

    unsigned int ImageView::n_instances() const
    {
        return m_frame_counter.max_frame_index;
    }
}

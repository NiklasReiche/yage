#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "../Handle.h"

namespace yage::gl::vulkan
{
    class Instance;

    class ImageView
    {
    public:
        ImageView(std::weak_ptr<Instance> instance, const VkImageViewCreateInfo& create_info);

        ~ImageView();

        ImageView(const ImageView& other) = delete;

        ImageView(ImageView&& other) noexcept;

        ImageView& operator=(const ImageView& other) = delete;

        ImageView& operator=(ImageView&& other) noexcept;

        [[nodiscard]] VkImageView vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkDevice m_vk_device;
        VkImageView m_vk_handle{};
    };

    using ImageViewHandle = Handle<ImageView>;
}

#pragma once

#include <memory>

#include <vulkan/vulkan.h>

namespace yage::gl::vulkan
{
    class Instance;

    class ImageView
    {
    public:
        ImageView(std::weak_ptr<Instance> instance, const VkImageViewCreateInfo& create_info);

        ~ImageView();

        [[nodiscard]] VkImageView vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkImageView m_vk_handle{};
    };
}

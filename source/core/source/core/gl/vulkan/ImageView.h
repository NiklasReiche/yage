#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "FrameCounter.h"
#include "core/gl/enums.h"

namespace yage::gl::vulkan
{
    class Instance;

    // TODO: MAX_FRAMES_IN_FLIGHT is different from swap chain size
    class ImageView
    {
    public:
        // TODO: take shared ownership of VkImage Handle (or somehow raw vk handle for swap chain images)
        ImageView(std::weak_ptr<Instance> instance, std::span<VkImageViewCreateInfo> create_info, ResourceUsage usage, FrameCounter frame_counter);

        ~ImageView();

        ImageView(const ImageView& other) = delete;

        ImageView(ImageView&& other) noexcept;

        ImageView& operator=(const ImageView& other) = delete;

        ImageView& operator=(ImageView&& other) noexcept;

        [[nodiscard]] VkImageView vk_handle() const;

        [[nodiscard]] VkImageView vk_handle(std::size_t index) const;

        [[nodiscard]] std::size_t n_instances() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkDevice m_vk_device;
        ResourceUsage m_usage;
        std::vector<VkImageView> m_vk_handles{};
        FrameCounter m_frame_counter{};
    };

    using ImageViewHandle = Handle<ImageView>;
}

#pragma once

#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "../ITexture2D.h"
#include "../enums.h"
#include "FrameCounter.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Texture2D final : public ITexture2D2
    {
    public:
        Texture2D(Instance* instance, FrameCounter frame_counter, const TextureSampler& sampler,
                  const PixelTransferInfo& data_info, std::span<const std::byte> data);

        ~Texture2D() override;

        Texture2D(const Texture2D& other) = delete;

        Texture2D(Texture2D&& other) noexcept;

        Texture2D& operator=(const Texture2D& other) = delete;

        Texture2D& operator=(Texture2D&& other) noexcept;

        [[nodiscard]] VkSampler vk_sampler() const;

        [[nodiscard]] std::vector<VkDescriptorImageInfo> descriptor_info() const;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device = VK_NULL_HANDLE;
        FrameCounter m_frame_counter;

        std::vector<VkDeviceMemory> m_vk_memories;
        std::vector<VkImage> m_vk_images;
        std::vector<VkImageView> m_vk_image_views;
        VkSampler m_vk_sampler = VK_NULL_HANDLE; // TODO: cache and reuse identical samplers

        unsigned int m_width = 0;
        unsigned int m_height = 0;
        unsigned int m_mip_levels = 0;
    };
}

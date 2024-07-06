#pragma once

#include <memory>
#include <span>

#include <vulkan/vulkan.h>

#include "../ITexture2D.h"
#include "../enums.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Texture2D final : public ITexture2D2
    {
    public:
        Texture2D(Instance* instance, const PixelTransferInfo& data_info, std::span<const std::byte> data);

        ~Texture2D() override;

        Texture2D(const Texture2D& other) = delete;

        Texture2D(Texture2D&& other) noexcept;

        Texture2D& operator=(const Texture2D& other) = delete;

        Texture2D& operator=(Texture2D&& other) noexcept;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        VkImage m_image_handle{};
        VkDeviceMemory m_memory_handle{};
    };
}

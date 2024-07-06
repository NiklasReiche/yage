#include "Texture2D.h"
#include "Instance.h"
#include "enums.h"

namespace yage::gl::vulkan
{
    Texture2D::Texture2D(Instance* instance, const PixelTransferInfo& data_info,
                         const std::span<const std::byte> data)
        : m_instance(instance),
          m_vk_device(m_instance->device())
    {
        const VkDeviceSize buffer_size = data.size();

        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        m_instance->create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                     staging_buffer, staging_buffer_memory);

        void* mapped_data;
        vkMapMemory(m_vk_device, staging_buffer_memory, 0, buffer_size, 0, &mapped_data);
        memcpy(mapped_data, data.data(), buffer_size);
        vkUnmapMemory(m_vk_device, staging_buffer_memory);

        const VkFormat vk_image_format = convert(data_info.image_format);

        m_instance->create_image(data_info.width, data_info.height, vk_image_format, VK_IMAGE_TILING_OPTIMAL,
                                    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image_handle, m_memory_handle);

        m_instance->transition_image_layout(m_image_handle, vk_image_format, VK_IMAGE_LAYOUT_UNDEFINED,
                                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        m_instance->copy_buffer_to_image(staging_buffer, m_image_handle, data_info.width, data_info.height);

        m_instance->transition_image_layout(m_image_handle, vk_image_format,
                                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(m_vk_device, staging_buffer, nullptr);
        vkFreeMemory(m_vk_device, staging_buffer_memory, nullptr);
    }

    Texture2D::~Texture2D()
    {
        if (m_image_handle != VK_NULL_HANDLE) {
            vkDestroyImage(m_vk_device, m_image_handle, nullptr);
            vkFreeMemory(m_vk_device, m_memory_handle, nullptr);
        }
    }

    Texture2D::Texture2D(Texture2D&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_image_handle(other.m_image_handle),
          m_memory_handle(other.m_memory_handle)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_image_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;
    }

    Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_image_handle = other.m_image_handle;
        m_memory_handle = other.m_memory_handle;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_image_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;

        return *this;
    }
}

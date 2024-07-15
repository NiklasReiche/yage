#include "IndexBuffer.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    IndexBuffer::IndexBuffer(Instance* instance, const IndexDataInfo data_info, const std::span<const std::byte> data)
        : m_instance(instance),
          m_vk_device(m_instance->device()),
          m_index_count(data_info.index_count)
    {
        const VkDeviceSize buffer_size = data.size();

        // TODO: code duplication with vertex buffer
        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        m_instance->create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                  staging_buffer, staging_buffer_memory);

        void* mapped_data;
        vkMapMemory(m_vk_device, staging_buffer_memory, 0, buffer_size, 0, &mapped_data);
        memcpy(mapped_data, data.data(), buffer_size);
        vkUnmapMemory(m_vk_device, staging_buffer_memory);

        m_instance->create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer_handle, m_memory_handle);

        m_instance->copy_buffer(staging_buffer, m_buffer_handle, buffer_size);

        vkDestroyBuffer(m_vk_device, staging_buffer, nullptr);
        vkFreeMemory(m_vk_device, staging_buffer_memory, nullptr);
    }

    IndexBuffer::~IndexBuffer()
    {
        clear();
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_buffer_handle(other.m_buffer_handle),
          m_memory_handle(other.m_memory_handle),
          m_index_count(other.m_index_count)
    {
        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;
        other.m_index_count = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_buffer_handle = other.m_buffer_handle;
        m_memory_handle = other.m_memory_handle;
        m_index_count = other.m_index_count;

        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;
        other.m_index_count = 0;

        return *this;
    }

    VkBuffer IndexBuffer::vk_handle() const
    {
        return m_buffer_handle;
    }

    void IndexBuffer::clear()
    {
        if (m_buffer_handle != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_vk_device, m_buffer_handle, nullptr);
        }
        if (m_memory_handle != VK_NULL_HANDLE) {
            vkFreeMemory(m_vk_device, m_memory_handle, nullptr);
        }
    }
}

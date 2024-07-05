#include "VertexBuffer.h"
#include "Instance.h"

#include <utility>

namespace yage::gl::vulkan
{
    VertexBuffer::VertexBuffer(std::weak_ptr<Instance> instance, const VertexDataInfo& data_info,
                               const std::span<const std::byte>& data)
        : m_instance(std::move(instance)),
          m_vk_device(m_instance.lock()->device()),
          m_vertex_count(gl::vertex_count(data_info, data))
    {
        const auto instance_lock = m_instance.lock();
        const VkDeviceSize buffer_size = data.size();

        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        instance_lock->create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer,
                      staging_buffer_memory);

        void* mapped_data;
        vkMapMemory(m_vk_device, staging_buffer_memory, 0, buffer_size, 0, &mapped_data);
        memcpy(mapped_data, data.data(), buffer_size);
        vkUnmapMemory(m_vk_device, staging_buffer_memory);

        instance_lock->create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer_handle, m_memory_handle);

        instance_lock->copy_buffer(staging_buffer, m_buffer_handle, buffer_size);

        vkDestroyBuffer(m_vk_device, staging_buffer, nullptr);
        vkFreeMemory(m_vk_device, staging_buffer_memory, nullptr);
    }

    VertexBuffer::~VertexBuffer()
    {
        if (m_buffer_handle != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_vk_device, m_buffer_handle, nullptr);
            vkFreeMemory(m_vk_device, m_memory_handle, nullptr);
        }
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        : m_instance(std::move(other.m_instance)),
          m_vk_device(other.m_vk_device),
          m_buffer_handle(other.m_buffer_handle),
          m_memory_handle(other.m_memory_handle),
          m_vertex_count(other.m_vertex_count)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = std::move(other.m_instance);
        m_vk_device = other.m_vk_device, m_buffer_handle = other.m_buffer_handle;
        m_memory_handle = other.m_memory_handle;
        m_vertex_count = other.m_vertex_count;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;

        return *this;
    }

    VkBuffer VertexBuffer::vk_handle() const
    {
        return m_buffer_handle;
    }

    std::size_t VertexBuffer::vertex_count() const
    {
        return m_vertex_count;
    }
}

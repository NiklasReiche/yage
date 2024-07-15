#include "VertexBuffer.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    VertexBuffer::VertexBuffer(Instance* instance, const FrameCounter frame_counter, const VertexDataInfo& data_info,
                               const std::span<const std::byte>& data)
        : m_instance(instance),
          m_vk_device(m_instance->device()),
          m_frame_counter(frame_counter),
          m_vertex_count(gl::vertex_count(data_info, data))
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

        m_buffer_handles.resize(m_frame_counter.max_frame_index);
        m_memory_handles.resize(m_frame_counter.max_frame_index);
        for (std::size_t i = 0; i < m_frame_counter.max_frame_index; i++) {
            // TODO: for dynamic buffers we probably don't want VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            m_instance->create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer_handles[i], m_memory_handles[i]);

            m_instance->copy_buffer(staging_buffer, m_buffer_handles[i], buffer_size);
        }

        vkDestroyBuffer(m_vk_device, staging_buffer, nullptr);
        vkFreeMemory(m_vk_device, staging_buffer_memory, nullptr);
    }

    VertexBuffer::~VertexBuffer()
    {
        clear();
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_frame_counter(other.m_frame_counter),
          m_buffer_handles(std::move(other.m_buffer_handles)),
          m_memory_handles(std::move(other.m_memory_handles)),
          m_vertex_count(other.m_vertex_count)
    {
        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vertex_count = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_frame_counter = other.m_frame_counter;
        m_buffer_handles = std::move(other.m_buffer_handles);
        m_memory_handles = std::move(other.m_memory_handles);
        m_vertex_count = other.m_vertex_count;

        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vertex_count = 0;

        return *this;
    }

    VkBuffer VertexBuffer::vk_handle() const
    {
        return m_buffer_handles[*m_frame_counter.curent_frame_index];
    }

    std::size_t VertexBuffer::vertex_count() const
    {
        return m_vertex_count;
    }

    void VertexBuffer::clear()
    {
        for (std::size_t i = 0; i < m_frame_counter.max_frame_index; ++i) {
            vkDestroyBuffer(m_vk_device, m_buffer_handles[i], nullptr);
            vkFreeMemory(m_vk_device, m_memory_handles[i], nullptr);
        }
    }
}

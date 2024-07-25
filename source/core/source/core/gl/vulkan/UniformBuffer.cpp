#include "UniformBuffer.h"
#include "Instance.h"
#include <cstring>

namespace yage::gl::vulkan
{
    UniformBuffer::UniformBuffer(Instance* m_instance, const FrameCounter frame_counter,
                                 const std::size_t data_byte_size)
        : m_instance(m_instance),
          m_vk_device(m_instance->device()),
          m_frame_counter(frame_counter),
          m_buffer_size(data_byte_size)
    {
        m_vk_buffers.resize(m_frame_counter.max_frame_index);
        m_vk_memories.resize(m_frame_counter.max_frame_index);
        m_buffers_mapped.resize(m_frame_counter.max_frame_index);

        for (std::size_t i = 0; i < m_frame_counter.max_frame_index; i++) {
            m_instance->create_buffer(m_buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                      m_vk_buffers[i], m_vk_memories[i]);

            vkMapMemory(m_vk_device, m_vk_memories[i], 0, m_buffer_size, 0, &m_buffers_mapped[i]);
        }
    }

    UniformBuffer::~UniformBuffer()
    {
        clear();
    }

    UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_frame_counter(other.m_frame_counter),
          m_buffer_size(other.m_buffer_size),
          m_vk_buffers(std::move(other.m_vk_buffers)),
          m_vk_memories(std::move(other.m_vk_memories)),
          m_buffers_mapped(std::move(other.m_buffers_mapped))
    {
        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_size = 0;
    }

    UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_frame_counter = other.m_frame_counter;
        m_buffer_size = other.m_buffer_size;
        m_vk_buffers = std::move(other.m_vk_buffers);
        m_vk_memories = std::move(other.m_vk_memories);
        m_buffers_mapped = std::move(other.m_buffers_mapped);

        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_size = 0;

        return *this;
    }

    void UniformBuffer::update_sub_data(const std::size_t byte_offset, const std::size_t byte_size, const void* data)
    {
        std::memcpy(static_cast<std::byte*>(m_buffers_mapped[*m_frame_counter.curent_frame_index]) + byte_offset, data, byte_size);
    }

    std::vector<VkDescriptorBufferInfo> UniformBuffer::descriptor_info() const
    {
        std::vector<VkDescriptorBufferInfo> buffer_infos;
        buffer_infos.reserve(m_frame_counter.max_frame_index);
        for (unsigned int i = 0; i < m_frame_counter.max_frame_index; ++i) {
            buffer_infos.emplace_back(VkDescriptorBufferInfo{
                    .buffer = m_vk_buffers[*m_frame_counter.curent_frame_index], .offset = 0, .range = m_buffer_size});
        }
        return buffer_infos;
    }

    void UniformBuffer::clear()
    {
        for (size_t i = 0; i < m_frame_counter.max_frame_index; i++) {
            vkDestroyBuffer(m_vk_device, m_vk_buffers[i], nullptr);
            vkFreeMemory(m_vk_device, m_vk_memories[i], nullptr);
        }
        m_vk_buffers.clear();
        m_vk_memories.clear();
    }
}

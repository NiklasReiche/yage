#include "VertexBuffer.h"
#include "Instance.h"

#include <utility>

namespace yage::gl::vulkan
{
    VertexBuffer::VertexBuffer(std::weak_ptr<Instance> instance, const std::span<const std::byte>& data,
                               const std::size_t vertex_count)
        : m_instance(std::move(instance)),
          m_vk_device(m_instance.lock()->device()),
          m_vertex_count(vertex_count)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = data.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferInfo.flags = 0;
        if (vkCreateBuffer(m_vk_device, &bufferInfo, nullptr, &m_buffer_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_vk_device, m_buffer_handle, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex =
                findMemoryType(memRequirements.memoryTypeBits,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if (vkAllocateMemory(m_vk_device, &allocInfo, nullptr, &m_memory_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(m_vk_device, m_buffer_handle, m_memory_handle, 0);

        void* raw_data;
        vkMapMemory(m_vk_device, m_memory_handle, 0, bufferInfo.size, 0, &raw_data);
        memcpy(raw_data, data.data(), bufferInfo.size);
        vkUnmapMemory(m_vk_device, m_memory_handle);
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
        m_vk_device = other.m_vk_device,
        m_buffer_handle = other.m_buffer_handle;
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

    std::uint32_t VertexBuffer::findMemoryType(const std::uint32_t typeFilter, const VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_instance.lock()->physical_device(), &memProperties);

        for (std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("Vulkan: failed to find suitable memory type!");
    }
}

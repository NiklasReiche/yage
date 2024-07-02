#include "VertexBuffer.h"
#include "Instance.h"

#include <utility>

namespace yage::gl::vulkan
{
    VertexBuffer::VertexBuffer(std::weak_ptr<Instance> instance, const std::span<const std::byte>& data, const std::size_t vertex_count)
            : m_instance(std::move(instance)), m_vertex_count(vertex_count)
    {
        VkDevice device = m_instance.lock()->device();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = data.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferInfo.flags = 0;
        if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_buffer_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, m_buffer_handle, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if (vkAllocateMemory(device, &allocInfo, nullptr, &m_memory_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(device, m_buffer_handle, m_memory_handle, 0);

        void* raw_data;
        vkMapMemory(device, m_memory_handle, 0, bufferInfo.size, 0, &raw_data);
        memcpy(raw_data, data.data(), bufferInfo.size);
        vkUnmapMemory(device, m_memory_handle);
    }

    VertexBuffer::~VertexBuffer()
    {
        const auto device = m_instance.lock()->device();
        vkDestroyBuffer(device, m_buffer_handle, nullptr);
        vkFreeMemory(device, m_memory_handle, nullptr);
    }

    VkBuffer VertexBuffer::vk_handle() const
    {
        return m_buffer_handle;
    }

    std::size_t VertexBuffer::vertex_count() const
    {
        return m_vertex_count;
    }

    std::uint32_t VertexBuffer::findMemoryType(const std::uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

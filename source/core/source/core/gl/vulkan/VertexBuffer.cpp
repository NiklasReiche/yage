#include "VertexBuffer.h"

#include <vulkan/vulkan.h>

#include <utility>

namespace gl::vulkan
{
    VertexBuffer::VertexBuffer(std::weak_ptr<Instance> instance, const std::span<const std::byte>& vertices)
            : m_instance(std::move(instance))
    {
        auto device = m_instance.lock()->device();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = vertices.size();
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

        void* data;
        vkMapMemory(device, m_memory_handle, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), bufferInfo.size);
        vkUnmapMemory(device, m_memory_handle);
    }

    VertexBuffer::~VertexBuffer()
    {
        auto device = m_instance.lock()->device();
        vkDestroyBuffer(device, m_buffer_handle, nullptr);
        vkFreeMemory(device, m_memory_handle, nullptr);
    }

    std::uint32_t VertexBuffer::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

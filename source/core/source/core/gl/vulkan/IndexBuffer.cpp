#include "IndexBuffer.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    IndexBuffer::IndexBuffer(std::weak_ptr<Instance> instance, const IndexDataInfo data_info,
                             const std::span<const std::byte> data)
        : m_instance(std::move(instance)),
          m_vk_device(m_instance.lock()->device()),
          m_index_count(data_info.index_count)
    {
        const VkDeviceSize buffer_size = data.size();

        // TODO: code duplication with vertex buffer
        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer,
                      staging_buffer_memory);

        void* mapped_data;
        vkMapMemory(m_vk_device, staging_buffer_memory, 0, buffer_size, 0, &mapped_data);
        memcpy(mapped_data, data.data(), buffer_size);
        vkUnmapMemory(m_vk_device, staging_buffer_memory);

        create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer_handle, m_memory_handle);

        copyBuffer(staging_buffer, m_buffer_handle, buffer_size);

        vkDestroyBuffer(m_vk_device, staging_buffer, nullptr);
        vkFreeMemory(m_vk_device, staging_buffer_memory, nullptr);
    }

    IndexBuffer::~IndexBuffer()
    {
        if (m_buffer_handle != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_vk_device, m_buffer_handle, nullptr);
            vkFreeMemory(m_vk_device, m_memory_handle, nullptr);
        }
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        : m_instance(std::move(other.m_instance)),
          m_vk_device(other.m_vk_device),
          m_buffer_handle(other.m_buffer_handle),
          m_memory_handle(other.m_memory_handle),
          m_index_count(other.m_index_count)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = std::move(other.m_instance);
        m_vk_device = other.m_vk_device;
        m_buffer_handle = other.m_buffer_handle;
        m_memory_handle = other.m_memory_handle;
        m_index_count = other.m_index_count;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_buffer_handle = VK_NULL_HANDLE;
        other.m_memory_handle = VK_NULL_HANDLE;

        return *this;
    }

    VkBuffer IndexBuffer::vk_handle() const
    {
        return m_buffer_handle;
    }

    std::uint32_t IndexBuffer::find_memory_type(const std::uint32_t typeFilter, const VkMemoryPropertyFlags properties)
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

    void IndexBuffer::create_buffer(const VkDeviceSize size, const VkBufferUsageFlags usage,
                                    const VkMemoryPropertyFlags properties, VkBuffer& buffer,
                                    VkDeviceMemory& buffer_memory)
    {
        VkBufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.size = size;
        create_info.usage = usage;
        create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.flags = 0;
        if (vkCreateBuffer(m_vk_device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_vk_device, buffer, &memRequirements);

        // TODO: don't call vkAllocateMemory for every buffer; implement an allocator or use VulkanMemoryAllocator
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = find_memory_type(memRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(m_vk_device, &allocInfo, nullptr, &buffer_memory) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(m_vk_device, buffer, buffer_memory, 0);
    }

    void IndexBuffer::copyBuffer(const VkBuffer source, const VkBuffer destination, const VkDeviceSize size)
    {
        const auto instance = m_instance.lock();

        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandPool = instance->command_pool();
        alloc_info.commandBufferCount = 1;

        VkCommandBuffer command_buffer;
        vkAllocateCommandBuffers(m_vk_device, &alloc_info, &command_buffer);

        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(command_buffer, &begin_info);

        VkBufferCopy copy_region{};
        copy_region.srcOffset = 0; // Optional
        copy_region.dstOffset = 0; // Optional
        copy_region.size = size;
        vkCmdCopyBuffer(command_buffer, source, destination, 1, &copy_region);

        vkEndCommandBuffer(command_buffer);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        // we could use fences here and schedule multiple vertex buffer creations together
        vkQueueSubmit(instance->graphics_queue(), 1, &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(instance->graphics_queue());

        vkFreeCommandBuffers(m_vk_device, instance->command_pool(), 1, &command_buffer);
    }
}

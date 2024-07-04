#pragma once

#include <cstdint>
#include <memory>
#include <span>

#include <vulkan/vulkan.h>

#include "../IIndexBuffer.h"
#include "../enums.h"

namespace yage::gl::vulkan
{
    class Instance;

    class IndexBuffer final : public IIndexBuffer
    {
    public:
        IndexBuffer(std::weak_ptr<Instance> instance, IndexDataInfo data_info, std::span<const std::byte> data);

        ~IndexBuffer() override;

        IndexBuffer(const IndexBuffer& other) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept;

        IndexBuffer& operator=(const IndexBuffer& other) = delete;

        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        [[nodiscard]] VkBuffer vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance{};
        VkDevice m_vk_device;
        VkBuffer m_buffer_handle{};
        VkDeviceMemory m_memory_handle{};
        std::size_t m_index_count = 0;

        std::uint32_t find_memory_type(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                           VkBuffer& buffer, VkDeviceMemory& buffer_memory);

        void copyBuffer(VkBuffer source, VkBuffer destination, VkDeviceSize size);
    };
}

#pragma once

#include <span>
#include <memory>
#include <cstdint>

#include <vulkan/vulkan.h>

namespace yage::gl::vulkan
{
    class Instance;

    class VertexBuffer final
    {
    public:
        explicit VertexBuffer(std::weak_ptr<Instance> instance, const std::span<const std::byte>& data, std::size_t vertex_count);

        ~VertexBuffer();

        [[nodiscard]] VkBuffer vk_handle() const;

        [[nodiscard]] std::size_t vertex_count() const;

    private:
        std::weak_ptr<Instance> m_instance{};
        VkBuffer m_buffer_handle{};
        VkDeviceMemory m_memory_handle{};
        std::size_t m_vertex_count = 0;

        std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}

#pragma once

#include <span>
#include <memory>
#include <cstdint>

#include "Instance.h"

namespace yage::gl::vulkan
{
    class VertexBuffer final
    {
    public:
        explicit VertexBuffer(std::weak_ptr<Instance> instance, const std::span<const std::byte>& data);

        ~VertexBuffer();

    private:
        std::weak_ptr<Instance> m_instance{};
        VkBuffer m_buffer_handle{};
        VkDeviceMemory m_memory_handle{};

        std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}

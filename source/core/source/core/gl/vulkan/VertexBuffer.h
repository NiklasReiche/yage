#pragma once

#include <span>
#include <memory>
#include <cstdint>

#include "Instance.h"

namespace gl::vulkan
{
    class VertexBuffer
    {
    public:
        explicit VertexBuffer(std::weak_ptr<Instance> instance, const std::span<const std::byte>& vertices);

        virtual ~VertexBuffer();

    private:
        std::weak_ptr<Instance> m_instance{}; // TODO: type of dependency
        VkBuffer m_buffer_handle{};
        VkDeviceMemory m_memory_handle{};

        std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}

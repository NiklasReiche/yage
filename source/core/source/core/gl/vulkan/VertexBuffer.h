#pragma once

#include <cstdint>
#include <memory>
#include <span>

#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "../IVertexBuffer.h"
#include "../enums.h"

namespace yage::gl::vulkan
{
    class Instance;

    class VertexBuffer final : public IVertexBuffer
    {
    public:
        explicit VertexBuffer(std::weak_ptr<Instance> instance, const VertexDataInfo& data_info,
                              const std::span<const std::byte>& data);

        ~VertexBuffer();

        VertexBuffer(const VertexBuffer& other) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept;

        VertexBuffer& operator=(const VertexBuffer& other) = delete;

        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        [[nodiscard]] VkBuffer vk_handle() const;

        [[nodiscard]] std::size_t vertex_count() const;

    private:
        std::weak_ptr<Instance> m_instance{};
        VkDevice m_vk_device;
        VkBuffer m_buffer_handle{};
        VkDeviceMemory m_memory_handle{};
        std::size_t m_vertex_count = 0;
    };

    using VertexBufferHandle = Handle<VertexBuffer>;
}

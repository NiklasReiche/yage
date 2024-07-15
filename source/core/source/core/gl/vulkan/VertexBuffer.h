#pragma once

#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "../IVertexBuffer.h"
#include "../enums.h"
#include "FrameCounter.h"

namespace yage::gl::vulkan
{
    class Instance;

    class VertexBuffer final : public IVertexBuffer
    {
    public:
        explicit VertexBuffer(Instance* instance, FrameCounter frame_counter, const VertexDataInfo& data_info,
                              const std::span<const std::byte>& data);

        ~VertexBuffer() override;

        VertexBuffer(const VertexBuffer& other) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept;

        VertexBuffer& operator=(const VertexBuffer& other) = delete;

        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        [[nodiscard]] VkBuffer vk_handle() const;

        [[nodiscard]] std::size_t vertex_count() const;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        std::vector<VkBuffer> m_buffer_handles;
        std::vector<VkDeviceMemory> m_memory_handles;
        std::size_t m_vertex_count = 0;

        void clear();
    };
}

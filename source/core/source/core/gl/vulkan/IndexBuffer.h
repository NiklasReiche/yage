#pragma once

#include <span>
#include <vector>

#include <vulkan/vulkan.h>

#include "../IIndexBuffer.h"
#include "../enums.h"
#include "FrameCounter.h"

namespace yage::gl::vulkan
{
    class Instance;

    class IndexBuffer final : public IIndexBuffer
    {
    public:
        IndexBuffer(Instance* instance, FrameCounter frame_counter, IndexDataInfo data_info,
                    std::span<const std::byte> data);

        ~IndexBuffer() override;

        IndexBuffer(const IndexBuffer& other) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept;

        IndexBuffer& operator=(const IndexBuffer& other) = delete;

        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        [[nodiscard]] VkBuffer vk_handle() const;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        std::vector<VkBuffer> m_buffer_handles;
        std::vector<VkDeviceMemory> m_memory_handles;
        std::size_t m_index_count = 0;

        void clear();
    };
}

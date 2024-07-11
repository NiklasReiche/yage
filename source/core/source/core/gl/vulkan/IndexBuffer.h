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
        IndexBuffer(Instance* instance, IndexDataInfo data_info, std::span<const std::byte> data);

        ~IndexBuffer() override;

        IndexBuffer(const IndexBuffer& other) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept;

        IndexBuffer& operator=(const IndexBuffer& other) = delete;

        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        [[nodiscard]] VkBuffer vk_handle() const;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        VkBuffer m_buffer_handle{};
        VkDeviceMemory m_memory_handle{}; // TODO: multi-buffering
        std::size_t m_index_count = 0;
    };
}

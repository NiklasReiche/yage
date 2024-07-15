#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "../IUniformBuffer.h"
#include "FrameCounter.h"

namespace yage::gl::vulkan
{
    class Instance;

    class UniformBuffer final : public IUniformBuffer
    {
    public:
        UniformBuffer(Instance* m_instance, FrameCounter frame_counter, std::size_t data_byte_size);

        ~UniformBuffer() override;

        UniformBuffer(const UniformBuffer& other) = delete;

        UniformBuffer(UniformBuffer&& other) noexcept;

        UniformBuffer& operator=(const UniformBuffer& other) = delete;

        UniformBuffer& operator=(UniformBuffer&& other) noexcept;

        void update_data(std::size_t size, const void* data) override;

        void update_sub_data(std::size_t byte_offset, std::size_t byte_size, const void* data) override;

        [[nodiscard]] std::vector<VkDescriptorBufferInfo> descriptor_info() const;

    private:
        Instance* m_instance;
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        std::size_t m_buffer_size;
        std::vector<VkBuffer> m_vk_buffers;
        std::vector<VkDeviceMemory> m_vk_memories;
        std::vector<void*> m_buffers_mapped;

        void clear();
    };
}

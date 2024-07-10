#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "FrameCounter.h"

namespace yage::gl::vulkan
{
    class Instance;

    class UniformBuffer final
    {
    public:
        UniformBuffer(Instance* m_instance, FrameCounter frame_counter, std::size_t data_byte_size);

        ~UniformBuffer();

        void update_data(std::size_t size, const void* data);

        void update_sub_data(std::size_t offset, std::size_t size, const void* data);

        [[nodiscard]] std::vector<VkDescriptorBufferInfo> descriptor_info() const;

    private:
        Instance* m_instance;
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        std::size_t m_buffer_size;
        std::vector<VkBuffer> m_vk_buffers;
        std::vector<VkDeviceMemory> m_vk_memories;
        std::vector<void*> m_buffers_mapped;
    };
}

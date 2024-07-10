#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "DescriptorSetLayout.h"
#include "FrameCounter.h"
#include "UniformBuffer.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DescriptorSet
    {
    public:
        DescriptorSet(Instance* instance, const VkDescriptorSetAllocateInfo& alloc_info, FrameCounter frame_counter, std::shared_ptr<Handle<DescriptorSetLayout>> layout);

        void write(std::uint32_t binding, const std::shared_ptr<Handle<UniformBuffer>>& uniform_buffer);

        [[nodiscard]] const VkDescriptorSet& vk_handle() const;

        [[nodiscard]] VkDescriptorSetLayout vk_layout() const;

    private:
        Instance* m_instance;
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        std::vector<VkDescriptorSet> m_vk_handles;

        // dependant resources
        std::shared_ptr<Handle<DescriptorSetLayout>> m_layout;
        std::vector<std::shared_ptr<Handle<UniformBuffer>>> m_bound_uniform_buffers;
    };
}

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "../IDescriptorSet.h"
#include "DescriptorSetLayout.h"
#include "FrameCounter.h"
#include "UniformBuffer.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DescriptorSet final : public IDescriptorSet
    {
    public:
        DescriptorSet(Instance* instance, const VkDescriptorSetAllocateInfo& alloc_info, FrameCounter frame_counter,
                      DescriptorSetLayoutSharedHandle layout);

        // descriptor sets are destroyed automatically when the descriptor pool dies
        ~DescriptorSet() override = default;

        void write(unsigned int binding, const UniformBufferSharedHandle& uniform_buffer) override;

        [[nodiscard]] const VkDescriptorSet& vk_handle() const;

        [[nodiscard]] VkDescriptorSetLayout vk_layout() const;

    private:
        Instance* m_instance;
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        std::vector<VkDescriptorSet> m_vk_handles;

        // dependant resources
        DescriptorSetLayoutSharedHandle m_layout;
        std::unordered_map<std::uint32_t, UniformBufferSharedHandle> m_bound_uniform_buffers;
    };
}

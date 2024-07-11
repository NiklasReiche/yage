#pragma once

#include <span>

#include <vulkan/vulkan.h>

#include "DescriptorSet.h"

namespace yage::gl::vulkan
{
    class Instance;

    using DescriptorSetStore = Store<IDescriptorSet, DescriptorSet>;

    class DescriptorAllocator
    {
    public:
        struct PoolSizeRatio
        {
            VkDescriptorType type;
            float ratio;
        };

        explicit DescriptorAllocator(Instance* instance, std::uint32_t max_sets, std::span<PoolSizeRatio> pool_ratios);

        ~DescriptorAllocator();

        DescriptorAllocator(const DescriptorAllocator& other) = delete;

        DescriptorAllocator(DescriptorAllocator&& other) noexcept;

        DescriptorAllocator& operator=(const DescriptorAllocator& other) = delete;

        DescriptorAllocator& operator=(DescriptorAllocator&& other) noexcept;

        void clear();

        void reset();

        [[nodiscard]] DescriptorSetStore& store_descriptor_sets();

        [[nodiscard]] VkDescriptorPool vk_pool() const;

    private:
        Instance* m_instance;
        VkDevice m_vk_device = VK_NULL_HANDLE;
        VkDescriptorPool m_vk_pool = VK_NULL_HANDLE;

        std::shared_ptr<DescriptorSetStore> m_store_descriptor_sets = std::make_shared<DescriptorSetStore>();
    };
}

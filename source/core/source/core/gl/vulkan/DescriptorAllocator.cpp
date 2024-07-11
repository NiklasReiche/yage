#include "DescriptorAllocator.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    DescriptorAllocator::DescriptorAllocator(Instance* instance, const std::uint32_t max_sets,
                                             std::span<PoolSizeRatio> pool_ratios)
        : m_instance(instance), m_vk_device(instance->device())
    {
        std::vector<VkDescriptorPoolSize> pool_sizes;
        for (const PoolSizeRatio ratio: pool_ratios) {
            pool_sizes.push_back(VkDescriptorPoolSize{
                    .type = ratio.type,
                    .descriptorCount = static_cast<std::uint32_t>(ratio.ratio * static_cast<float>(max_sets))});
        }

        VkDescriptorPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = 0;
        pool_info.maxSets = max_sets;
        pool_info.poolSizeCount = static_cast<std::uint32_t>(pool_sizes.size());
        pool_info.pPoolSizes = pool_sizes.data();

        if (vkCreateDescriptorPool(m_vk_device, &pool_info, nullptr, &m_vk_pool) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create descriptor pool");
        }
    }

    DescriptorAllocator::~DescriptorAllocator()
    {
        reset();
    }

    DescriptorAllocator::DescriptorAllocator(DescriptorAllocator&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_vk_pool(other.m_vk_pool),
          m_store_descriptor_sets(std::move(other.m_store_descriptor_sets))
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_pool = VK_NULL_HANDLE;
    }

    DescriptorAllocator& DescriptorAllocator::operator=(DescriptorAllocator&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_vk_pool = other.m_vk_pool;
        m_store_descriptor_sets = std::move(other.m_store_descriptor_sets);

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_pool = VK_NULL_HANDLE;

        return *this;
    }

    void DescriptorAllocator::clear()
    {
        if (m_vk_pool != VK_NULL_HANDLE) {
            vkResetDescriptorPool(m_vk_device, m_vk_pool, 0);
        }
    }

    void DescriptorAllocator::reset()
    {
        if (m_vk_pool != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(m_vk_device, m_vk_pool, nullptr);
        }
        m_vk_pool = VK_NULL_HANDLE;
    }

    DescriptorSetStore& DescriptorAllocator::store_descriptor_sets()
    {
        return *m_store_descriptor_sets;
    }

    VkDescriptorPool DescriptorAllocator::vk_pool() const
    {
        return m_vk_pool;
    }
}

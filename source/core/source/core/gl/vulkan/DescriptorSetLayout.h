#pragma once

#include <vulkan/vulkan.h>

#include "../IDescriptorSetLayout.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DescriptorSetLayout : public IDescriptorSetLayout
    {
    public:
        explicit DescriptorSetLayout(Instance* instance, const VkDescriptorSetLayoutCreateInfo& create_info);

        ~DescriptorSetLayout();

        [[nodiscard]] const VkDescriptorSetLayout& vk_handle() const;

    private:
        Instance* m_instance = nullptr;
        VkDevice m_vk_device = VK_NULL_HANDLE;

        VkDescriptorSetLayout m_vk_handle = VK_NULL_HANDLE;
    };
}

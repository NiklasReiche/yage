#pragma once

#include <memory>
#include <vector>

#include "../Handle.h"
#include "DescriptorSet.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DescriptorSetLayoutBuilder
    {
    public:
        explicit DescriptorSetLayoutBuilder(std::weak_ptr<Instance> instance);

        DescriptorSetLayoutBuilder& with_uniform_buffer();

        DescriptorSetLayoutBuilder& with_uniform_buffer_array(std::uint32_t count);

        Handle<DescriptorSetLayout> build();

        void clear();

    private:
        std::weak_ptr<Instance> m_instance;

        std::uint32_t m_binding_number = 0;
        std::vector<VkDescriptorSetLayoutBinding> m_bindings;
        VkDescriptorSetLayoutCreateInfo m_create_info{};
    };
}

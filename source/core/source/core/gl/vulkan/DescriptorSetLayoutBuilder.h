#pragma once

#include <memory>
#include <vector>

#include "../Handle.h"
#include "../IDescriptorSetLayoutBuilder.h"
#include "DescriptorSet.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DescriptorSetLayoutBuilder final : public IDescriptorSetLayoutBuilder
    {
    public:
        explicit DescriptorSetLayoutBuilder(std::weak_ptr<Instance> instance);

        IDescriptorSetLayoutBuilder& with_uniform_buffer_at(unsigned int binding) override;

        DescriptorSetLayoutBuilder& with_uniform_buffer_array_at(unsigned int binding, std::uint32_t count);

        DescriptorSetLayoutHandle build() override;

        void clear() override;

    private:
        std::weak_ptr<Instance> m_instance;

        std::vector<VkDescriptorSetLayoutBinding> m_bindings;
        VkDescriptorSetLayoutCreateInfo m_create_info{};
    };
}

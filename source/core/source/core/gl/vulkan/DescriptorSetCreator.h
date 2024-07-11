#pragma once

#include <memory>

#include "../IDescriptorSetCreator.h"
#include "../enums.h"
#include "DescriptorSet.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DescriptorSetCreator final : public IDescriptorSetCreator
    {
    public:
        explicit DescriptorSetCreator(std::weak_ptr<Instance> instance);

        DescriptorSetHandle create(ResourceUsage usage, DescriptorSetLayoutHandle layout) override;

    private:
        std::weak_ptr<Instance> m_instance;
    };
}

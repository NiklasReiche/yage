#pragma once

#include <memory>

#include "../enums.h"
#include "DescriptorSet.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DescriptorSetCreator
    {
    public:
        explicit DescriptorSetCreator(std::weak_ptr<Instance> instance);

        Handle<DescriptorSet> create(ResourceUsage usage, const std::shared_ptr<Handle<DescriptorSetLayout>>& layout);

    private:
        std::weak_ptr<Instance> m_instance;
    };
}

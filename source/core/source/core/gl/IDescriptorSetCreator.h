#pragma once

#include "IDescriptorSet.h"
#include "IDescriptorSetLayout.h"
#include "enums.h"

namespace yage::gl
{
    class IDescriptorSetCreator
    {
    public:
        virtual ~IDescriptorSetCreator() = default;

        virtual DescriptorSetHandle create(ResourceUsage usage, const DescriptorSetLayoutSharedHandle& layout) = 0;
    };
}

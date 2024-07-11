#pragma once
#include "Handle.h"

namespace yage::gl
{
    class IDescriptorSetLayout
    {
    public:
        ~IDescriptorSetLayout() = default;
    };

    using DescriptorSetLayoutHandle = Handle<IDescriptorSetLayout>;
    using DescriptorSetLayoutSharedHandle = std::shared_ptr<Handle<IDescriptorSetLayout>>;
}

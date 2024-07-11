#pragma once

#include <memory>

#include "Handle.h"
#include "IUniformBuffer.h"

namespace yage::gl
{
    class IDescriptorSet
    {
    public:
        virtual ~IDescriptorSet() = default;

        virtual void write(unsigned int binding, const UniformBufferSharedHandle& uniform_buffer) = 0;
    };

    using DescriptorSetHandle = Handle<IDescriptorSet>;
    using DescriptorSetSharedHandle = std::shared_ptr<Handle<IDescriptorSet>>;
}

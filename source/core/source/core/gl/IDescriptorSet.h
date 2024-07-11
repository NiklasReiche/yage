#pragma once

#include "Handle.h"
#include "IUniformBuffer.h"

namespace yage::gl
{
    class IDescriptorSet
    {
    public:
        virtual ~IDescriptorSet() = default;

        virtual void write(unsigned int binding, const UniformBufferHandle& uniform_buffer) = 0;
    };

    using DescriptorSetHandle = Handle<IDescriptorSet>;
}

#pragma once

#include "Handle.h"
#include "ITexture2D.h"
#include "IUniformBuffer.h"

namespace yage::gl
{
    class IDescriptorSet
    {
    public:
        virtual ~IDescriptorSet() = default;

        virtual void write(unsigned int binding, const UniformBufferHandle& uniform_buffer) = 0;

        virtual void write(unsigned int binding, const Texture2DHandle& texture_2d) = 0;
    };

    using DescriptorSetHandle = Handle<IDescriptorSet>;
}

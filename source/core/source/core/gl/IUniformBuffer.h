#pragma once

#include "Handle.h"

namespace yage::gl
{
    class IUniformBuffer
    {
    public:
        virtual ~IUniformBuffer() = default;

        virtual void update_data(std::size_t size, const void* data) = 0;

        virtual void update_sub_data(std::size_t byte_offset, std::size_t byte_size, const void* data) = 0;
    };

    using UniformBufferHandle = Handle<IUniformBuffer>;
}

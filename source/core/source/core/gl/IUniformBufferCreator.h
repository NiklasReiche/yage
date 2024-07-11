#pragma once

#include <span>

#include "enums.h"
#include "Handle.h"
#include "IUniformBuffer.h"

namespace yage::gl
{
    class IUniformBufferCreator
    {
    public:
        virtual ~IUniformBufferCreator() = default;

        [[nodiscard]] virtual UniformBufferHandle create(std::size_t byte_size, ResourceUsage usage) const = 0;
    };
}

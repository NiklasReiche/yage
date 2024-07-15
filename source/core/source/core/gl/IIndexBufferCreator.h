#pragma once

#include <span>

#include "IIndexBuffer.h"
#include "enums.h"

namespace yage::gl
{
    class IIndexBufferCreator
    {
    public:
        virtual ~IIndexBufferCreator() = default;

        [[nodiscard]] virtual IndexBufferHandle create(IndexDataInfo data_info, std::span<const std::byte> data,
                                                       ResourceUsage usage) const = 0;
    };
}

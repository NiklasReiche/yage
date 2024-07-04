#pragma once

#include <span>

#include "Handle.h"
#include "IIndexBuffer.h"
#include "enums.h"

namespace yage::gl
{
    class IIndexBufferCreator
    {
    public:
        virtual ~IIndexBufferCreator() = default;

        [[nodiscard]] virtual Handle<IIndexBuffer> create(IndexDataInfo data_info,
                                                          std::span<const std::byte> data) const = 0;
    };
}

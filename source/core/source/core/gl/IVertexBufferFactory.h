#pragma once

#include <span>

#include "Handle.h"
#include "IVertexBuffer.h"
#include "enums.h"

namespace yage::gl
{
    class IVertexBufferFactory
    {
    public:
        virtual ~IVertexBufferFactory() = default;

        [[nodiscard]] virtual Handle<IVertexBuffer> create(const VertexDataInfo& data_info,
                                                           std::span<const std::byte> data) const = 0;
    };
}

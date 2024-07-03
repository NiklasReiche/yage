#pragma once

#include <span>

#include "Handle.h"
#include "IVertexBuffer.h"

namespace yage::gl
{
    class IVertexBufferFactory
    {
    public:
        virtual ~IVertexBufferFactory() = default;

        virtual Handle<IVertexBuffer> create_test(const std::span<const std::byte>& data, std::size_t vertex_count) = 0;
    };
}

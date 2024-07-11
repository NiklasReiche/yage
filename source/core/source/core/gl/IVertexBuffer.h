#pragma once

#include "Handle.h"

namespace yage::gl
{
    class IVertexBuffer
    {
    public:
        virtual ~IVertexBuffer() = default;
    };

    using VertexBufferHandle = Handle<IVertexBuffer>;
}

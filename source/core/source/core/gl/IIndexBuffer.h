#pragma once

#include "Handle.h"

namespace yage::gl
{
    class IIndexBuffer
    {
    public:
        virtual ~IIndexBuffer() = default;
    };

    using IndexBufferHandle = Handle<IIndexBuffer>;
}

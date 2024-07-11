#pragma once

#include "Handle.h"

namespace yage::gl
{
    class IFrameBuffer
    {
    public:
        virtual ~IFrameBuffer() = default;
    };

    using FrameBufferHandle = Handle<IFrameBuffer>;
}

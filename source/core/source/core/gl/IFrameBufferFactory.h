#pragma once

#include "Handle.h"
#include "IFrameBuffer.h"

namespace yage::gl
{
    class IFrameBufferFactory
    {
    public:
        virtual ~IFrameBufferFactory() = default;
    };
}

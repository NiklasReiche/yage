#pragma once

#include "Handle.h"
#include "IFrameBuffer.h"

namespace yage::gl
{
    class IFrameBufferCreator
    {
    public:
        virtual ~IFrameBufferCreator() = default;
    };
}

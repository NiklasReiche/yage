#pragma once

#include "Handle.h"

namespace yage::gl
{
    class IRenderTarget
    {
    public:
        virtual ~IRenderTarget() = default;
    };

    using RenderTargetHandle = Handle<IRenderTarget>;
}

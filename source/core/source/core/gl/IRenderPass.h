#pragma once

#include "Handle.h"

namespace yage::gl
{
    class IRenderPass
    {
    public:
        virtual ~IRenderPass() = default;
    };

    using RenderPassHandle = Handle<IRenderPass>;
}

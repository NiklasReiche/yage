#pragma once

#include "Handle.h"
#include "enums.h"

namespace yage::gl
{
    class IPipeline
    {
    public:
        virtual ~IPipeline() = default;

        virtual void set_dynamic_viewport(Viewport viewport, ScissorRectangle scissor) = 0;
    };

    using PipelineHandle = Handle<IPipeline>;
}

#pragma once

#include "Handle.h"

namespace yage::gl
{
    class IPipeline
    {
    public:
        virtual ~IPipeline() = default;
    };

    using PipelineHandle = Handle<IPipeline>;
}

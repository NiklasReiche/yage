#pragma once

#include "IDescriptorSet.h"
#include "IDrawable.h"
#include "IPipeline.h"
#include "IRenderTarget.h"

namespace yage::gl
{
    class IRenderer2
    {
    public:
        virtual ~IRenderer2() = default;

        virtual void begin_command_buffer() = 0;

        virtual void begin_render_pass() = 0;

        virtual void begin_render_pass(const IRenderTarget& render_target) = 0;

        virtual void bind_descriptor_set(const IDescriptorSet& descriptor_set) = 0;

        virtual void bind_pipeline(const IPipeline& pipeline) = 0;

        virtual void draw(const IDrawable2& drawable) = 0;

        virtual void end_render_pass() = 0;

        virtual void end_command_buffer() = 0;
    };
}

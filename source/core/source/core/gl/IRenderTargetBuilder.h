#pragma once

#include "Handle.h"
#include "IRenderTarget.h"
#include "enums.h"

namespace yage::gl
{
    class IRenderTargetBuilder
    {
    public:
        virtual ~IRenderTargetBuilder() = default;

        virtual IRenderTargetBuilder& with_msaa(MSAASamples samples) = 0;

        virtual IRenderTargetBuilder& with_color_attachment(TextureFormat2 format) = 0;

        virtual IRenderTargetBuilder& with_depth_attachment(TextureFormat2 format) = 0;

        virtual IRenderTargetBuilder& with_resolve_attachments() = 0;

        // TODO: fixed-width types
        virtual RenderTargetHandle build(unsigned int width, unsigned int height, ResourceUsage usage) = 0;

        virtual void clear() = 0;
    };
}

#pragma once

#include "enums.h"
#include "IRenderPass.h"

namespace yage::gl
{
    class IRenderPassBuilder
    {
    public:
        virtual ~IRenderPassBuilder() = default;

        virtual IRenderPassBuilder& with_msaa(MSAASamples samples) = 0;

        virtual IRenderPassBuilder& with_color_attachment(ImageFormat2 format) = 0;

        virtual IRenderPassBuilder& with_depth_attachment(ImageFormat2 format) = 0;

        virtual IRenderPassBuilder& with_resolve_attachments() = 0;

        virtual RenderPassHandle build() = 0;

        virtual void clear() = 0;
    };
}

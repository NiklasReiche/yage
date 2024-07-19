#pragma once

#include "../IRenderTargetBuilder.h"

namespace yage::gl::opengl4
{
    class Context;

    class RenderTargetBuilder final : public IRenderTargetBuilder
    {
    public:
        explicit RenderTargetBuilder(Context* context);

        IRenderTargetBuilder& with_msaa(MSAASamples samples) override;

        IRenderTargetBuilder& with_color_attachment(TextureFormat2 format) override;

        IRenderTargetBuilder& with_depth_attachment(TextureFormat2 format) override;

        IRenderTargetBuilder& with_resolve_attachments() override;

        RenderTargetHandle build(unsigned width, unsigned height, ResourceUsage usage) override;

        void clear() override;

    private:
        Context* m_context; // TODO

        MSAASamples m_samples = MSAASamples::SAMPLE_1;
        std::vector<TextureFormat2> m_color_attachments;
        std::optional<TextureFormat2> m_depth_attachment;
        bool m_with_resolve;
    };
}

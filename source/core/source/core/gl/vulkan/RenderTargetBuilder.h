#pragma once

#include <memory>

#include "../Handle.h"
#include "../IRenderTargetBuilder.h"
#include "RenderTarget.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderTargetBuilder final : public IRenderTargetBuilder
    {
    public:
        explicit RenderTargetBuilder(std::weak_ptr<Instance> instance);

        IRenderTargetBuilder& with_msaa(MSAASamples samples) override;

        IRenderTargetBuilder& with_color_attachment(TextureFormat2 format) override;

        IRenderTargetBuilder& with_depth_attachment(TextureFormat2 format) override;

        IRenderTargetBuilder& with_resolve_attachments() override;

        RenderTargetHandle build(unsigned int width, unsigned int height, ResourceUsage usage) override;

        void clear() override;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance

        MSAASamples m_samples = MSAASamples::SAMPLE_1;

        TextureFormat2 m_depth_image_format = TextureFormat2::UNDEFINED;
        std::vector<TextureFormat2> m_color_image_formats;

        std::vector<VkRenderingAttachmentInfo> m_color_attachment_descriptions;
        std::optional<VkRenderingAttachmentInfo> m_depth_attachment_description;

        bool m_with_resolve = false;
    };
}

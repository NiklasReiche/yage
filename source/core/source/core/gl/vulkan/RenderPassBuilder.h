#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "../IRenderPassBuilder.h"
#include "../enums.h"
#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderPassBuilder final : public IRenderPassBuilder
    {
    public:
        explicit RenderPassBuilder(std::weak_ptr<Instance> instance);

        IRenderPassBuilder& with_msaa(MSAASamples samples) override;

        IRenderPassBuilder& with_color_attachment(ImageFormat2 format) override;

        IRenderPassBuilder& with_depth_attachment(ImageFormat2 format) override;

        IRenderPassBuilder& with_resolve_attachments() override;

        RenderPassHandle build() override;

        void clear() override;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance

        VkFormat m_depth_image_format = VK_FORMAT_UNDEFINED;
        std::vector<VkFormat> m_color_image_formats;
        VkSampleCountFlagBits m_msaa_samples = VK_SAMPLE_COUNT_1_BIT;

        unsigned int m_attachment_count = 0;
        std::vector<VkAttachmentDescription> m_color_attachment_descriptions;
        std::vector<VkAttachmentDescription> m_resolve_attachment_descriptions;
        std::optional<VkAttachmentDescription> m_depth_attachment_description;

        std::vector<VkAttachmentReference> m_color_attachment_references;
        std::vector<VkAttachmentReference> m_resolve_attachment_references;
        std::optional<VkAttachmentReference> m_depth_attachment_reference;
    };
}

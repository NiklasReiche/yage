#include "RenderTargetBuilder.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    RenderTargetBuilder::RenderTargetBuilder(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_msaa(const MSAASamples samples)
    {
        m_samples = samples;

        return *this;
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_color_attachment(const ImageFormat2 format)
    {
        if (m_with_resolve) {
            throw std::logic_error(
                    "RenderTargetBuilder: cannot call with_color_attachment() after with_resolve_attachments()");
        }

        m_color_image_formats.push_back(format);

        VkRenderingAttachmentInfo attachment_info{};
        attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;

        attachment_info.imageView = VK_NULL_HANDLE; // gets filled in RenderTarget constructor
        attachment_info.imageLayout = VK_IMAGE_LAYOUT_GENERAL; // TODO

        attachment_info.resolveMode = VK_RESOLVE_MODE_NONE; // gets filled in with_resolve_attachments()
        attachment_info.resolveImageView = VK_NULL_HANDLE; // gets filled in RenderTarget constructor
        attachment_info.resolveImageLayout = VK_IMAGE_LAYOUT_GENERAL; // TODO

        attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment_info.clearValue = {0, 0, 0, 0}; // TODO

        return *this;
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_depth_attachment(const ImageFormat2 format)
    {
        m_depth_image_format = format;

        VkRenderingAttachmentInfo attachment_info{};
        attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;

        attachment_info.imageView = VK_NULL_HANDLE; // gets filled in RenderTarget constructor
        attachment_info.imageLayout = VK_IMAGE_LAYOUT_GENERAL; // TODO

        attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_info.clearValue.depthStencil.depth = 1.0f; // TODO

        m_depth_attachment_description = attachment_info;

        return *this;
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_resolve_attachments()
    {
        for (auto& attachment_info: m_color_attachment_descriptions) {
            attachment_info.resolveMode = VK_RESOLVE_MODE_AVERAGE_BIT;
        }

        return *this;
    }

    RenderTargetHandle RenderTargetBuilder::build(const unsigned int width, const unsigned int height,
                                                  const ResourceUsage usage)
    {
        std::vector<std::tuple<VkRenderingAttachmentInfo, ImageFormat2>> color_attachments;
        for (std::size_t i = 0; i < m_color_attachment_descriptions.size(); ++i) {
            color_attachments.emplace_back(m_color_attachment_descriptions[i], m_color_image_formats[i]);
        }

        std::optional<std::tuple<VkRenderingAttachmentInfo, ImageFormat2>> depth_attachment;
        if (m_depth_attachment_description.has_value()) {
            depth_attachment = std::make_tuple(m_depth_attachment_description.value(), m_depth_image_format);
        }

        const auto instance = m_instance.lock();
        return instance->store_render_targets().create(instance.get(), instance->frame_counter_for_usage(usage), width,
                                                       height, m_samples, color_attachments, depth_attachment);
    }

    void RenderTargetBuilder::clear()
    {
        m_samples = MSAASamples::SAMPLE_1;

        m_depth_image_format = ImageFormat2::UNDEFINED;
        m_color_image_formats.clear();

        m_color_attachment_descriptions.clear();
        m_depth_attachment_description.reset();
    }
}

#include "RenderPassBuilder.h"

#include <vulkan/vulkan.h>

#include "Instance.h"
#include "enums.h"

namespace yage::gl::vulkan
{
    RenderPassBuilder::RenderPassBuilder(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    IRenderPassBuilder& RenderPassBuilder::with_msaa(const MSAASamples samples)
    {
        m_msaa_samples = convert(samples);

        for (VkAttachmentDescription& description: m_color_attachment_descriptions) {
            description.samples = m_msaa_samples;
        }
        if (m_depth_attachment_description.has_value()) {
            m_depth_attachment_description.value().samples = m_msaa_samples;
        }

        return *this;
    }

    IRenderPassBuilder& RenderPassBuilder::with_color_attachment(const ImageFormat2 format)
    {
        if (!m_resolve_attachment_descriptions.empty()) {
            throw std::logic_error(
                    "RenderPassBuilder: cannot call with_color_attachment() after with_resolve_attachments()");
        }

        const VkFormat vk_image_format = convert(format);
        m_color_image_formats.push_back(vk_image_format);

        VkAttachmentDescription color_attachment{};
        color_attachment.format = vk_image_format;
        color_attachment.samples = m_msaa_samples;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        m_color_attachment_descriptions.push_back(color_attachment);

        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = m_attachment_count;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        m_color_attachment_references.push_back(color_attachment_ref);

        m_attachment_count++;

        return *this;
    }

    IRenderPassBuilder& RenderPassBuilder::with_depth_attachment(const ImageFormat2 format)
    {
        const VkFormat vk_image_format = convert(format);
        m_depth_image_format = vk_image_format;

        VkAttachmentDescription depth_attachment{};
        depth_attachment.format = vk_image_format;
        depth_attachment.samples = m_msaa_samples;
        depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        m_depth_attachment_description = depth_attachment;

        VkAttachmentReference depth_attachment_ref{};
        depth_attachment_ref.attachment = m_attachment_count;
        depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        m_depth_attachment_reference = depth_attachment_ref;

        m_attachment_count++;

        return *this;
    }

    IRenderPassBuilder& RenderPassBuilder::with_resolve_attachments()
    {
        for (const VkAttachmentDescription& color_attachment_description: m_color_attachment_descriptions) {
            VkAttachmentDescription color_resolve_attachment{};
            color_resolve_attachment.format = color_attachment_description.format;
            color_resolve_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            color_resolve_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_resolve_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_resolve_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_resolve_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            color_resolve_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_resolve_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // TODO: ?

            m_resolve_attachment_descriptions.push_back(color_resolve_attachment);

            VkAttachmentReference color_resolve_attachment_ref{};
            color_resolve_attachment_ref.attachment = m_attachment_count;
            color_resolve_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            m_resolve_attachment_references.push_back(color_resolve_attachment_ref);

            m_attachment_count++;
        }

        return *this;
    }

    RenderPassHandle RenderPassBuilder::build()
    {
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = m_color_attachment_references.size();
        subpass.pColorAttachments = m_color_attachment_references.data();
        subpass.pResolveAttachments = m_resolve_attachment_references.data();
        if (m_depth_attachment_reference.has_value()) {
            subpass.pDepthStencilAttachment = &m_depth_attachment_reference.value();
        } else {
            subpass.pDepthStencilAttachment = nullptr;
        }

        std::vector<VkAttachmentDescription> attachment_descriptions;
        attachment_descriptions.reserve(m_color_attachment_descriptions.size() +
                                                        m_resolve_attachment_descriptions.size() +
                                                        m_depth_attachment_description.has_value()
                                                ? 1
                                                : 0);

        attachment_descriptions.insert(attachment_descriptions.end(), m_color_attachment_descriptions.begin(),
                                       m_color_attachment_descriptions.end());
        if (m_depth_attachment_description.has_value()) {
            attachment_descriptions.push_back(m_depth_attachment_description.value());
        }
        attachment_descriptions.insert(attachment_descriptions.end(), m_resolve_attachment_descriptions.begin(),
                                       m_resolve_attachment_descriptions.end());

        VkRenderPassCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        create_info.attachmentCount = static_cast<std::uint32_t>(attachment_descriptions.size());
        create_info.pAttachments = attachment_descriptions.data();
        create_info.subpassCount = 1;
        create_info.pSubpasses = &subpass;

        VkPipelineStageFlags stage_flags = 0;
        VkAccessFlags access_flags = 0;
        if (!m_color_attachment_references.empty()) {
            stage_flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            access_flags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        }
        if (m_depth_attachment_reference.has_value()) {
            stage_flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            access_flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        }

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcStageMask = stage_flags;
        dependency.srcAccessMask = 0;
        dependency.dstSubpass = 0;
        dependency.dstStageMask = stage_flags;
        dependency.dstAccessMask = access_flags;

        create_info.dependencyCount = 1;
        create_info.pDependencies = &dependency;

        const auto instance = m_instance.lock();
        return instance->store_render_passes().create(instance.get(), create_info, m_msaa_samples);
    }

    void RenderPassBuilder::clear()
    {
        m_color_image_formats.clear();
        m_depth_image_format = VK_FORMAT_UNDEFINED;
        m_msaa_samples = VK_SAMPLE_COUNT_1_BIT;

        m_attachment_count = 0;
        m_color_attachment_descriptions.clear();
        m_resolve_attachment_descriptions.clear();
        m_depth_attachment_description.reset();

        m_color_attachment_references.clear();
        m_resolve_attachment_references.clear();
        m_depth_attachment_reference.reset();
    }
}

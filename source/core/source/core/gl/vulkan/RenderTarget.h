#pragma once

#include <vulkan/vulkan.h>

#include <math/vector.h>

#include "../Handle.h"
#include "../IRenderTarget.h"
#include "../enums.h"
#include "FrameCounter.h"
#include "ImageView.h"
#include "core/gl/ITexture2D.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderTarget final : public IRenderTarget
    {
    public:
        explicit RenderTarget(Instance* instance, FrameCounter frame_counter, unsigned int width, unsigned int height,
                              MSAASamples samples,
                              std::span<std::tuple<VkRenderingAttachmentInfo, ImageFormat2>> color_attachments,
                              std::optional<std::tuple<VkRenderingAttachmentInfo, ImageFormat2>> depth_attachment);

        ~RenderTarget() override = default;

        RenderTarget(const RenderTarget& other) = delete;

        RenderTarget(RenderTarget&& other) noexcept;

        RenderTarget& operator=(const RenderTarget& other) = delete;

        RenderTarget& operator=(RenderTarget&& other) noexcept;

        [[nodiscard]] unsigned int n_instances() const;

        [[nodiscard]] math::Vec2ui extent() const;

        [[nodiscard]] MSAASamples samples() const;

        [[nodiscard]] const std::vector<VkRenderingAttachmentInfo>& color_attachment_infos() const;

        [[nodiscard]] const std::vector<VkFormat>& color_attachment_formats() const;

        [[nodiscard]] const std::optional<VkRenderingAttachmentInfo>& depth_attachment_info() const;

        [[nodiscard]] const std::optional<VkFormat>& depth_attachment_format() const;

        // TODO: stencil attachment

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        std::vector<std::vector<VkRenderingAttachmentInfo>> m_color_attachment_infos;
        std::vector<VkFormat> m_color_attachment_formats;
        std::vector<Texture2DHandle> m_color_attachments;
        std::vector<Texture2DHandle> m_color_resolve_attachments;

        std::vector<std::optional<VkRenderingAttachmentInfo>> m_depth_attachment_info;
        std::optional<VkFormat> m_depth_attachment_format;
        std::optional<Texture2DHandle> m_depth_attachment;

        math::Vec2ui m_extent{};
        MSAASamples m_samples;
    };
}

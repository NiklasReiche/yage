#include "RenderTarget.h"
#include "Instance.h"
#include "enums.h"

namespace yage::gl::vulkan
{
    RenderTarget::RenderTarget(Instance* instance, const FrameCounter frame_counter, unsigned int width,
                               unsigned int height, const MSAASamples samples,
                               std::span<std::tuple<VkRenderingAttachmentInfo, TextureFormat2>> color_attachments,
                               std::optional<std::tuple<VkRenderingAttachmentInfo, TextureFormat2>> depth_attachment)
        : m_instance(instance),
          m_frame_counter(frame_counter),
          m_extent({width, height}),
          m_samples(samples)
    {
        const std::size_t n_instances = m_frame_counter.max_frame_index;
        m_color_attachment_infos.resize(n_instances);
        m_depth_attachment_info.resize(n_instances);
        m_color_attachment_formats.reserve(color_attachments.size());

        for (const auto& [info, format]: color_attachments) {
            TextureSampler sampler{.min_filter = TextureFilter2::LINEAR, .mag_filter = TextureFilter2::LINEAR};
            PixelTransferInfo data_info{
                    .image_format = format,
                    .width = width,
                    .height = height,
            };
            Texture2DHandle texture = m_instance->store_textures().create(
                    m_instance, m_frame_counter, sampler, data_info,
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, info.imageLayout,
                    convert(m_samples));
            m_color_attachments.push_back(std::move(texture));

            if (info.resolveMode != VK_RESOLVE_MODE_NONE) {
                Texture2DHandle resolve_texture = m_instance->store_textures().create(
                        m_instance, m_frame_counter, sampler, data_info,
                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, info.resolveImageLayout,
                        VK_SAMPLE_COUNT_1_BIT);
                m_color_resolve_attachments.push_back(std::move(resolve_texture));
            }

            for (std::size_t i = 0; i < n_instances; ++i) {
                m_color_attachment_infos[i].push_back(info);
            }
            m_color_attachment_formats.push_back(convert(format));
        }

        assert(m_color_attachments.size() == m_color_resolve_attachments.size());

        if (depth_attachment.has_value()) {
            const auto [info, format] = depth_attachment.value();

            TextureSampler sampler{};
            PixelTransferInfo data_info{
                    .image_format = format,
                    .width = width,
                    .height = height,
            };
            Texture2DHandle texture = m_instance->store_textures().create(
                    m_instance, m_frame_counter, sampler, data_info, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                    info.imageLayout, convert(m_samples));
            m_depth_attachment = std::move(texture);

            for (std::size_t i = 0; i < n_instances; ++i) {
                m_depth_attachment_info[i] = info;
            }
            m_depth_attachment_format = convert(format);
        }

        for (std::size_t i = 0; i < n_instances; ++i) {
            for (std::size_t j = 0; j < color_attachments.size(); ++j) {
                m_color_attachment_infos[i][j].imageView = m_color_attachments[j].get<Texture2D>().vk_image_view(i);
                if (m_color_attachment_infos[i][j].resolveMode != VK_RESOLVE_MODE_NONE) {
                    m_color_attachment_infos[i][j].resolveImageView =
                            m_color_resolve_attachments[j].get<Texture2D>().vk_image_view(i);
                }
            }

            if (depth_attachment.has_value()) {
                m_depth_attachment_info[i].value().imageView =
                        m_depth_attachment.value().get<Texture2D>().vk_image_view(i);
            }
        }
    }

    RenderTarget::RenderTarget(RenderTarget&& other) noexcept
        : m_instance(other.m_instance),
          m_frame_counter(other.m_frame_counter),
          m_color_attachment_infos(std::move(other.m_color_attachment_infos)),
          m_color_attachment_formats(std::move(other.m_color_attachment_formats)),
          m_color_attachments(std::move(other.m_color_attachments)),
          m_color_resolve_attachments(std::move(other.m_color_resolve_attachments)),
          m_depth_attachment_info(std::move(other.m_depth_attachment_info)),
          m_depth_attachment_format(other.m_depth_attachment_format),
          m_depth_attachment(std::move(other.m_depth_attachment)),
          m_extent(other.m_extent),
          m_samples(other.m_samples)
    {
        other.m_instance = nullptr;
        other.m_extent = {0, 0};
    }

    RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_instance = other.m_instance;
        m_frame_counter = other.m_frame_counter;
        m_extent = other.m_extent;
        m_samples = other.m_samples;

        other.m_instance = nullptr;
        other.m_extent = {0, 0};

        return *this;
    }

    unsigned int RenderTarget::n_instances() const
    {
        return m_frame_counter.max_frame_index;
    }

    math::Vec2ui RenderTarget::extent() const
    {
        return m_extent;
    }

    MSAASamples RenderTarget::samples() const
    {
        return m_samples;
    }

    const std::vector<VkRenderingAttachmentInfo>& RenderTarget::color_attachment_infos() const
    {
        return m_color_attachment_infos[*m_frame_counter.curent_frame_index];
    }

    const std::vector<VkFormat>& RenderTarget::color_attachment_formats() const
    {
        return m_color_attachment_formats;
    }

    const std::optional<VkRenderingAttachmentInfo>& RenderTarget::depth_attachment_info() const
    {
        return m_depth_attachment_info[*m_frame_counter.curent_frame_index];
    }

    const std::optional<VkFormat>& RenderTarget::depth_attachment_format() const
    {
        return m_depth_attachment_format;
    }
}

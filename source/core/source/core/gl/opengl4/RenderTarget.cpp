#include "RenderTarget.h"
#include "Context.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    RenderTarget::RenderTarget(Context* context, const unsigned int width, const unsigned int height,
                               const MSAASamples samples, const std::span<const TextureFormat2> color_attachments,
                               const std::optional<TextureFormat2> depth_attachment, const bool with_resolve)
        : m_context(context)
    {
        build_draw_buffer(width, height, samples, color_attachments, depth_attachment);
        if (with_resolve) {
            build_read_buffer(width, height, color_attachments);
        }
    }

    RenderTarget::~RenderTarget()
    {
        clear();
    }

    RenderTarget::RenderTarget(RenderTarget&& other) noexcept
        : m_context(other.m_context),
          m_fbo_handle(other.m_fbo_handle),
          m_color_attachments(std::move(other.m_color_attachments)),
          m_depth_attachment(other.m_depth_attachment)
    {
        other.m_context = nullptr;
        other.m_fbo_handle = 0;
        other.m_depth_attachment.reset();
    }

    RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_context = other.m_context;
        m_fbo_handle = other.m_fbo_handle;
        m_color_attachments = std::move(other.m_color_attachments);
        m_depth_attachment = other.m_depth_attachment;

        other.m_context = nullptr;
        other.m_fbo_handle = 0;
        other.m_depth_attachment.reset();

        return *this;
    }

    void RenderTarget::resolve_msaa()
    {
        if (m_fbo_resolve_handle != 0) {
            m_context->bind_frame_buffer(GL_READ_FRAMEBUFFER, m_fbo_handle);
            m_context->bind_frame_buffer(GL_DRAW_FRAMEBUFFER, m_fbo_resolve_handle);
            glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
    }

    GLuint RenderTarget::gl_draw_handle() const
    {
        return m_fbo_handle;
    }

    GLuint RenderTarget::gl_read_handle() const
    {
        return m_fbo_resolve_handle == 0 ? m_fbo_handle : m_fbo_resolve_handle;
    }

    void RenderTarget::build_draw_buffer(const unsigned int width, const unsigned int height, const MSAASamples samples,
                                         const std::span<const TextureFormat2> color_attachments,
                                         const std::optional<TextureFormat2> depth_attachment)
    {
        glGenFramebuffers(1, &m_fbo_handle);

        m_context->bind_frame_buffer(GL_FRAMEBUFFER, m_fbo_handle);

        const ITexture2DCreator& texture_creator = m_context->texture_2d_creator();
        constexpr TextureSampler sampler{
                .min_filter = TextureFilter2::LINEAR,
                .mag_filter = TextureFilter2::NEAREST,
                .mip_map_mode = MipMapMode::NONE,
                .u_wrapper = TextureWrapper2::REPEAT,
                .v_wrapper = TextureWrapper2::REPEAT,
        };
        for (std::size_t i = 0; i < color_attachments.size(); ++i) {
            PixelTransferInfo data_info{.image_format = color_attachments[i], .width = width, .height = height};

            if (samples == MSAASamples::SAMPLE_1) {
                m_color_attachments.push_back(texture_creator.create(sampler, data_info, {}, ResourceUsage::DYNAMIC));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                       m_color_attachments[i].get<Texture2D>().gl_handle(), 0);
            } else {
                m_color_attachments.push_back(
                        texture_creator.create_multisampled(samples, data_info, ResourceUsage::DYNAMIC));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE,
                                       m_color_attachments[i].get<Texture2D>().gl_handle(), 0);
            }
        }

        if (depth_attachment.has_value()) {
            GLuint rbo;
            glGenRenderbuffers(1, &rbo);

            m_context->bind_render_buffer(rbo);

            const GLenum internal_format = to_internal_format(depth_attachment.value());
            if (samples == MSAASamples::SAMPLE_1) {
                glRenderbufferStorage(GL_RENDERBUFFER, internal_format, static_cast<GLsizei>(width),
                                      static_cast<GLsizei>(height));
            } else {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, static_cast<GLsizei>(n_samples(samples)),
                                                 internal_format, static_cast<GLsizei>(width),
                                                 static_cast<GLsizei>(height));
            }

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

            m_depth_attachment = rbo;
        }

        // completeness check
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("OpenGL: frame buffer construction failed!");
        }
    }

    void RenderTarget::build_read_buffer(const unsigned int width, const unsigned int height,
                                         const std::span<const TextureFormat2> color_attachments)
    {
        glGenFramebuffers(1, &m_fbo_resolve_handle);

        m_context->bind_frame_buffer(GL_FRAMEBUFFER, m_fbo_resolve_handle);

        const ITexture2DCreator& texture_creator = m_context->texture_2d_creator();
        constexpr TextureSampler sampler{
                .min_filter = TextureFilter2::LINEAR,
                .mag_filter = TextureFilter2::NEAREST,
                .mip_map_mode = MipMapMode::NONE,
                .u_wrapper = TextureWrapper2::REPEAT,
                .v_wrapper = TextureWrapper2::REPEAT,
        };
        for (std::size_t i = 0; i < color_attachments.size(); ++i) {
            PixelTransferInfo data_info{.image_format = color_attachments[i], .width = width, .height = height};
            m_resolve_color_attachments.push_back(texture_creator.create(sampler, data_info, {}, ResourceUsage::DYNAMIC));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                   m_resolve_color_attachments[i].get<Texture2D>().gl_handle(), 0);
        }

        // completeness check
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("OpenGL: frame buffer construction failed!");
        }
    }

    void RenderTarget::clear()
    {
        if (m_context != nullptr) {
            glDeleteFramebuffers(1, &m_fbo_handle);
            m_context->unbind_frame_buffer(GL_FRAMEBUFFER, m_fbo_handle);

            if (m_fbo_resolve_handle != 0) {
                glDeleteFramebuffers(1, &m_fbo_resolve_handle);
                m_context->unbind_frame_buffer(GL_FRAMEBUFFER, m_fbo_resolve_handle);
            }

            if (m_depth_attachment.has_value()) {
                m_context->unbind_render_buffer(m_depth_attachment.value());
            }
        }
    }
}

#include "RenderTarget.h"
#include "Context.h"

namespace yage::gl::opengl4
{
    RenderTarget::RenderTarget(Context* context, const unsigned int width, const unsigned int height,
                               const MSAASamples samples, const std::span<const TextureFormat2> color_attachments,
                               const std::optional<GLenum> depth_attachment)
        : m_context(context)
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
            PixelTransferInfo data_info{.width = width, .height = height, .image_format = color_attachments[i]};

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

            if (samples == MSAASamples::SAMPLE_1) {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width),
                                      static_cast<GLsizei>(height));
            } else {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, static_cast<GLsizei>(n_samples(samples)),
                                                 GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width),
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

    GLuint RenderTarget::gl_handle() const
    {
        return m_fbo_handle;
    }

    void RenderTarget::clear()
    {
        if (m_context != nullptr) {
            glDeleteFramebuffers(1, &m_fbo_handle);
            m_context->unbind_frame_buffer(GL_FRAMEBUFFER, m_fbo_handle);
            if (m_depth_attachment.has_value()) {
                m_context->unbind_render_buffer(m_depth_attachment.value());
            }
            // don't delete textures, as they might be in use elsewhere
        }
    }
}

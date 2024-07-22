#pragma once

#include <vector>

#include <glad/gl.h>

#include "../IRenderTarget.h"
#include "../ITexture2D.h"
#include "../enums.h"

namespace yage::gl::opengl4
{
    class Context;

    class RenderTarget final : public IRenderTarget
    {
    public:
        RenderTarget(Context* context, unsigned int width, unsigned int height, MSAASamples samples,
                     std::span<const TextureFormat2> color_attachments, std::optional<TextureFormat2> depth_attachment,
                     bool with_resolve);

        ~RenderTarget() override;

        RenderTarget(const RenderTarget& other) = delete;

        RenderTarget(RenderTarget&& other) noexcept;

        RenderTarget& operator=(const RenderTarget& other) = delete;

        RenderTarget& operator=(RenderTarget&& other) noexcept;

        void resolve_msaa();

        [[nodiscard]] GLuint gl_draw_handle() const;

        [[nodiscard]] GLuint gl_read_handle() const;

    private:
        Context* m_context;

        GLuint m_fbo_handle = 0;
        GLuint m_fbo_resolve_handle = 0;
        std::vector<Texture2DHandle> m_color_attachments;
        std::vector<Texture2DHandle> m_resolve_color_attachments;
        std::optional<GLuint> m_depth_attachment;

        std::uint32_t m_width;
        std::uint32_t m_height;

        void build_draw_buffer(unsigned int width, unsigned int height, MSAASamples samples,
                               std::span<const TextureFormat2> color_attachments,
                               std::optional<TextureFormat2> depth_attachment);

        void build_read_buffer(unsigned int width, unsigned int height,
                               std::span<const TextureFormat2> color_attachments);

        void clear();
    };
}

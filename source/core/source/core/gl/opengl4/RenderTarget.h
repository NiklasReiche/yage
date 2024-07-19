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
                     std::span<const TextureFormat2> color_attachments, std::optional<GLenum> depth_attachment);

        ~RenderTarget() override;

        RenderTarget(const RenderTarget& other) = delete;

        RenderTarget(RenderTarget&& other) noexcept;

        RenderTarget& operator=(const RenderTarget& other) = delete;

        RenderTarget& operator=(RenderTarget&& other) noexcept;

        [[nodiscard]] GLuint gl_handle() const;

    private:
        Context* m_context;

        GLuint m_fbo_handle = 0;
        std::vector<Texture2DHandle> m_color_attachments;
        std::optional<GLuint> m_depth_attachment;

        void clear();
    };
}

#pragma once

#include <span>

#include <glad/gl.h>

#include "../ITexture2D.h"
#include "../enums.h"

namespace yage::gl::opengl4
{
    class Context;

    class Texture2D final : public ITexture2D2
    {
    public:
        Texture2D(Context* context, const TextureSampler& sampler, const PixelTransferInfo& data_info,
                  std::span<const std::byte> data);

        Texture2D(Context* context, MSAASamples samples, const PixelTransferInfo& data_info);

        ~Texture2D() override;

        Texture2D(const Texture2D& other) = delete;

        Texture2D(Texture2D&& other) noexcept;

        Texture2D& operator=(const Texture2D& other) = delete;

        Texture2D& operator=(Texture2D&& other) noexcept;

        [[nodiscard]] GLuint gl_handle() const;

    private:
        Context* m_context;

        GLuint m_texture_handle = 0;

        void clear();
    };
}

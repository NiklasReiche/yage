#pragma once

#include "../ITexture2DCreator.h"

namespace yage::gl::opengl4
{
    class Context;

    class Texture2DCreator final : public ITexture2DCreator
    {
    public:
        explicit Texture2DCreator(Context* context);

        [[nodiscard]] Texture2DHandle create(const TextureSampler& sampler, const PixelTransferInfo& data_info,
                                             std::span<const std::byte> data, ResourceUsage usage) const override;

    private:
        Context* m_context; // TODO
    };
}

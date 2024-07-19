#pragma once

#include <span>

#include "ITexture2D.h"
#include "enums.h"

namespace yage::gl
{
    class ITexture2DCreator
    {
    public:
        virtual ~ITexture2DCreator() = default;

        [[nodiscard]] virtual Texture2DHandle create(const TextureSampler& sampler, const PixelTransferInfo& data_info,
                                                     std::span<const std::byte> data, ResourceUsage usage) const = 0;

        [[nodiscard]] virtual Texture2DHandle
        create_multisampled(MSAASamples samples, const PixelTransferInfo& data_info, ResourceUsage usage) const = 0;
    };
}

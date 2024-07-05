#pragma once

#include <span>

#include "Handle.h"
#include "ITexture2D.h"
#include "enums.h"

namespace yage::gl
{
    class ITexture2DCreator
    {
    public:
        virtual ~ITexture2DCreator() = default;

        [[nodiscard]] virtual Handle<ITexture2D2> create(const PixelTransferInfo& data_info,
                                                         std::span<const std::byte> data) const = 0;
    };
}

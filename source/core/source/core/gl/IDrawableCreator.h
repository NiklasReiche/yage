#pragma once

#include <span>

#include "Handle.h"
#include "IDrawable.h"
#include "enums.h"

namespace yage::gl
{
    class IDrawableCreator2
    {
    public:
        virtual ~IDrawableCreator2() = default;

        [[nodiscard]] virtual DrawableHandle create(VertexDataInfo vertex_data_info,
                                                    std::span<const std::byte> vertices, IndexDataInfo index_data_info,
                                                    std::span<const std::byte> indices, ResourceUsage usage) const = 0;
    };
}

#pragma once

#include <span>

#include "IDrawable.h"
#include "Handle.h"
#include "enums.h"

namespace yage::gl
{
    class IDrawableCreator2
    {
    public:
        virtual ~IDrawableCreator2() = default;

        [[nodiscard]] virtual Handle<IDrawable2> create(VertexDataInfo vertex_data_info,
                                                        std::span<const std::byte> vertices,
                                                        IndexDataInfo index_data_info,
                                                        std::span<const std::byte> indices) const = 0;
    };
}

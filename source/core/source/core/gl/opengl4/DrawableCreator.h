#pragma once

#include "../IDrawableCreator.h"

namespace yage::gl::opengl4
{
    class Context;

    class DrawableCreator final : public IDrawableCreator2
    {
    public:
        explicit DrawableCreator(Context* context);

        [[nodiscard]] DrawableHandle create(VertexDataInfo vertex_data_info, std::span<const std::byte> vertices,
                                            IndexDataInfo index_data_info, std::span<const std::byte> indices,
                                            ResourceUsage usage) const override;

    private:
        Context* m_context; // TODO
    };
}

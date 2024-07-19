#pragma once

#include "../IVertexBufferCreator.h"

namespace yage::gl::opengl4
{
    class Context;

    class VertexBufferCreator final : public IVertexBufferCreator
    {
    public:
        explicit VertexBufferCreator(Context* context);

        [[nodiscard]] VertexBufferHandle create(const VertexDataInfo& data_info, std::span<const std::byte> data,
                                                ResourceUsage usage) const override;

    private:
        Context* m_context; // TODO
    };
}

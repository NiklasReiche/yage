#pragma once

#include "../IIndexBufferCreator.h"

namespace yage::gl::opengl4
{
    class Context;

    class IndexBufferCreator final : public IIndexBufferCreator
    {
    public:
        explicit IndexBufferCreator(Context* context);

        [[nodiscard]] IndexBufferHandle create(IndexDataInfo data_info, std::span<const std::byte> data,
                                               ResourceUsage usage) const override;

    private:
        Context* m_context; // TODO
    };
}

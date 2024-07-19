#include "VertexBufferCreator.h"

namespace yage::gl::opengl4
{
    VertexBufferCreator::VertexBufferCreator(Context* context)
        : m_context(context)
    {
    }

    VertexBufferHandle VertexBufferCreator::create(const VertexDataInfo& data_info, std::span<const std::byte> data,
                                                   ResourceUsage usage) const
    {

    }
}

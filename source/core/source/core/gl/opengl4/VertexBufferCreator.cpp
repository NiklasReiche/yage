#include "VertexBufferCreator.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    VertexBufferCreator::VertexBufferCreator(Context* context)
        : m_context(context)
    {
    }

    VertexBufferHandle VertexBufferCreator::create(const VertexDataInfo&, std::span<const std::byte> data,
                                                   ResourceUsage usage) const
    {
        return m_context->store_vertex_buffers().create(m_context, data, usage);
    }
}

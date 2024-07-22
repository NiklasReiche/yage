#include "IndexBufferCreator.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    IndexBufferCreator::IndexBufferCreator(Context* context)
        : m_context(context)
    {
    }

    IndexBufferHandle IndexBufferCreator::create(IndexDataInfo, std::span<const std::byte> data,
                                                 ResourceUsage usage) const
    {
        return m_context->store_index_buffers().create(m_context, data, usage);
    }
}

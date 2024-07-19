#include "UniformBufferCreator.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    UniformBufferCreator::UniformBufferCreator(Context* context)
        : m_context(context)
    {
    }

    UniformBufferHandle UniformBufferCreator::create(std::size_t byte_size, ResourceUsage usage) const
    {
        return m_context->store_uniform_buffers().create(m_context, byte_size, usage);
    }
}

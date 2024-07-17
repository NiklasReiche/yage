#include "IndexBuffer.h"

namespace yage::gl::opengl4
{
    IndexBuffer::IndexBuffer(Context* context, const std::span<const std::byte> indices, const ResourceUsage usage)
        : m_context(context)
    {
        glGenBuffers(1, &m_ebo_handle);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size()), indices.data(), GL_STATIC_DRAW);
    }
}

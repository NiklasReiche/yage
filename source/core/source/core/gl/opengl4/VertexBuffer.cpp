#include "VertexBuffer.h"

#include "Context.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    VertexBuffer::VertexBuffer(Context* context, const std::span<const std::byte> data, const ResourceUsage usage)
        : m_context(context)
    {
        glGenBuffers(1, &m_vbo_handle);

        m_context->bind_buffer(GL_ARRAY_BUFFER, m_vbo_handle);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size()), data.data(), convert(usage));
    }

    VertexBuffer::~VertexBuffer()
    {
        clear();
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        : m_context(other.m_context),
          m_vbo_handle(other.m_vbo_handle)
    {
        other.m_context = nullptr;
        other.m_vbo_handle = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_context = other.m_context;
        m_vbo_handle = other.m_vbo_handle;

        other.m_context = nullptr;
        other.m_vbo_handle = 0;

        return *this;
    }

    GLuint VertexBuffer::gl_handle() const
    {
        return m_vbo_handle;
    }

    void VertexBuffer::clear()
    {
        if (m_context != nullptr) {
            glDeleteBuffers(1, &m_vbo_handle);
            m_context->unbind_buffer(GL_ARRAY_BUFFER, m_vbo_handle);
        }
    }
}

#include "IndexBuffer.h"

#include "Context.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    IndexBuffer::IndexBuffer(Context* context, const std::span<const std::byte> data, const ResourceUsage usage)
        : m_context(context)
    {
        glGenBuffers(1, &m_ebo_handle);

        // unbind potential VAO so we don't modify unrelated data when binding the EBO
        m_context->bind_vao(0);

        m_context->bind_buffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size()), data.data(), convert(usage));
    }

    IndexBuffer::~IndexBuffer()
    {
        clear();
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        : m_context(other.m_context),
          m_ebo_handle(other.m_ebo_handle)
    {
        other.m_context = nullptr;
        other.m_ebo_handle = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_context = other.m_context;
        m_ebo_handle = other.m_ebo_handle;

        other.m_context = nullptr;
        other.m_ebo_handle = 0;

        return *this;
    }

    GLuint IndexBuffer::gl_handle() const
    {
        return m_ebo_handle;
    }

    void IndexBuffer::clear()
    {
        // only delete resources if this handle is in a valid state
        if (m_context != nullptr) {
            glDeleteBuffers(1, &m_ebo_handle);
            m_context->unbind_buffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_handle);
        }
    }
}

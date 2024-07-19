#include "UniformBuffer.h"

#include "Context.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    UniformBuffer::UniformBuffer(Context* context, const std::size_t buffer_size, const ResourceUsage usage)
        : m_context(context)
    {
        glGenBuffers(1, &m_ubo_handle);

        m_context->bind_buffer(GL_UNIFORM_BUFFER, m_ubo_handle);
        glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(buffer_size), nullptr, convert(usage));
    }

    UniformBuffer::~UniformBuffer()
    {
        clear();
    }

    UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
        : m_context(other.m_context),
          m_ubo_handle(other.m_ubo_handle)
    {
        other.m_context = nullptr;
        other.m_ubo_handle = 0;
    }

    UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_context = other.m_context;
        m_ubo_handle = other.m_ubo_handle;

        other.m_context = nullptr;
        other.m_ubo_handle = 0;

        return *this;
    }

    void UniformBuffer::update_sub_data(const std::size_t byte_offset, const std::size_t byte_size, const void* data)
    {
        m_context->bind_buffer(GL_UNIFORM_BUFFER, m_ubo_handle);
        glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(byte_offset), static_cast<GLsizeiptr>(byte_size),
                        data);
    }

    void UniformBuffer::clear()
    {
        if (m_context != nullptr) {
            glDeleteBuffers(1, &m_ubo_handle);
            m_context->unbind_buffer(GL_UNIFORM_BUFFER, m_ubo_handle);
        }
    }
}

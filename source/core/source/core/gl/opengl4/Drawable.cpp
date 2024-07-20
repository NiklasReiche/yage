#include "Drawable.h"
#include "Context.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    Drawable::Drawable(Context* context, DrawableDescriptor descriptor)
        : m_context(context),
          m_vertex_buffer(std::move(descriptor.vertex_buffer)),
          m_index_buffer(std::move(descriptor.index_buffer))
    {
        m_indices_count = descriptor.index_data_info.index_count;
        m_indices_data_type = convert(descriptor.index_data_info.data_type);

        glGenVertexArrays(1, &m_vao_handle);

        context->bind_vao(m_vao_handle);

        const unsigned int vertex_byte_size = byte_size(descriptor.vertex_data_info.vertex_description);
        unsigned int attribute_byte_offset = descriptor.vertex_buffer_offset;

        context->bind_buffer(GL_ARRAY_BUFFER, m_vertex_buffer.get<VertexBuffer>().gl_handle());
        switch (descriptor.vertex_data_info.data_layout) {
            case VertexDataLayout::INTERLEAVED: {
                for (unsigned int i = 0; i < descriptor.vertex_data_info.vertex_description.size(); i++) {
                    const VertexComponent& attribute = descriptor.vertex_data_info.vertex_description[i];

                    const GLenum type = convert(attribute.data_type);
                    const auto vector_size = static_cast<GLint>(attribute.n_components);
                    const auto stride = static_cast<GLsizei>(vertex_byte_size);
                    const auto* offset = reinterpret_cast<GLvoid*>(attribute_byte_offset);
                    glEnableVertexAttribArray(i);
                    switch (type) {
                        case GL_FLOAT:          glVertexAttribPointer(i, vector_size, type, GL_FALSE, stride, offset); break;
                        // don't convert doubles and integers to floats
                        case GL_DOUBLE:         glVertexAttribLPointer(i, vector_size, type, stride, offset); break;
                        case GL_SHORT:
                        case GL_UNSIGNED_SHORT:
                        case GL_INT:
                        case GL_UNSIGNED_INT:   glVertexAttribIPointer(i, vector_size, type, stride, offset); break;

                        default: throw std::invalid_argument("unsupported vertex attribute type");
                    }
                    attribute_byte_offset += byte_size(attribute);
                }
                break;
            }
            case VertexDataLayout::BATCHED: {
                for (unsigned int i = 0; i < descriptor.vertex_data_info.vertex_description.size(); i++) {
                    const VertexComponent& attribute = descriptor.vertex_data_info.vertex_description[i];
                    const std::size_t attribute_byte_size = byte_size(attribute);

                    const GLenum type = convert(attribute.data_type);
                    const auto vector_size = static_cast<GLint>(attribute.n_components);
                    const auto stride = static_cast<GLsizei>(attribute_byte_size);
                    const auto* offset = reinterpret_cast<GLvoid*>(attribute_byte_offset);
                    glEnableVertexAttribArray(i);
                    switch (type) {
                        case GL_FLOAT:          glVertexAttribPointer(i, vector_size, type, GL_FALSE, stride, offset); break;
                        // don't convert doubles and integers to floats
                        case GL_DOUBLE:         glVertexAttribLPointer(i, vector_size, type, stride, offset); break;
                        case GL_SHORT:
                        case GL_UNSIGNED_SHORT:
                        case GL_INT:
                        case GL_UNSIGNED_INT:   glVertexAttribIPointer(i, vector_size, type, stride, offset); break;

                        default: throw std::invalid_argument("unsupported vertex attribute type");
                    }
                    attribute_byte_offset += attribute_byte_size * descriptor.n_vertices;
                }
                break;
            }
            default: throw std::invalid_argument("Unsupported vertex format");
        }

        // force (re-)binding of EBO so that VAO registers it
        context->force_bind_buffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer.get<IndexBuffer>().gl_handle());
    }

    Drawable::~Drawable()
    {
        clear();
    }

    Drawable::Drawable(Drawable&& other) noexcept
        : m_context(other.m_context),
          m_vao_handle(other.m_vao_handle),
          m_vertex_buffer(std::move(other.m_vertex_buffer)),
          m_index_buffer(std::move(other.m_index_buffer))
    {
        other.m_context = nullptr;
        other.m_vao_handle = 0;
    }

    Drawable& Drawable::operator=(Drawable&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_context = other.m_context;
        m_vao_handle = other.m_vao_handle;
        m_vertex_buffer = std::move(other.m_vertex_buffer);
        m_index_buffer = std::move(other.m_index_buffer);

        other.m_context = nullptr;
        other.m_vao_handle = 0;

        return *this;
    }

    GLuint Drawable::gl_vao_handle() const
    {
        return m_vao_handle;
    }

    GLenum Drawable::gl_index_type() const
    {
        return m_indices_data_type;
    }

    std::int32_t Drawable::indices_count() const
    {
        return m_indices_count;
    }

    void Drawable::clear()
    {
        if (m_context != nullptr) {
            glDeleteVertexArrays(1, &m_vao_handle);
            m_context->unbind_vao(m_vao_handle);
        }
    }
}

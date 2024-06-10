#include "DrawableCreator.h"

#include "Context.h"
#include "Drawable.h"
#include "OpenGL.h"
#include <memory>
#include <numeric>

namespace yage::opengl
{
    std::unique_ptr<gl::IDrawable>
    DrawableCreator::createDrawable(
            const std::span<const float>& vertices,
            const std::span<const unsigned int>& indices,
            const std::span<const unsigned int>& vertex_layout,
            gl::VertexFormat format)
    {
        auto context = lockContextPtr();
        auto drawable = std::make_unique<Drawable>();

        const unsigned int vertex_size = std::accumulate(vertex_layout.begin(), vertex_layout.end(), 0u);
        drawable->nIndices = vertex_size == 0 ? 0 : indices.size();
        const auto n_vertices = vertex_size == 0 ? 0 : vertices.size() / vertex_size;

        const std::shared_ptr<gl::VertexBuffer> vertex_buffer = createVertexBuffer(vertices);
        const std::shared_ptr<gl::ElementBuffer> element_buffer = createElementBuffer(indices);
        drawable->vertexArray = createVertexArrayInternal(vertex_buffer, element_buffer, vertex_layout, n_vertices,
                                                          format);

        return drawable;
    }

    std::unique_ptr<gl::IDrawable> DrawableCreator::createDrawable(
            const std::span<const std::byte>& vertices,
            const std::span<const std::byte>& indices,
            const std::span<const unsigned int>& vertex_layout,
            unsigned int n_indices,
            gl::VertexFormat format)
    {
        auto context = lockContextPtr();
        auto drawable = std::make_unique<Drawable>();

        const std::shared_ptr<gl::VertexBuffer> vertexBuffer = createVertexBuffer(vertices);
        const std::shared_ptr<gl::ElementBuffer> elementBuffer = createElementBuffer(indices);

        const unsigned int vertex_size = std::accumulate(vertex_layout.begin(), vertex_layout.end(), 0u);
        const int n_vertices = vertices.size() / (vertex_size * sizeof(float));

        drawable->vertexArray = createVertexArrayInternal(vertexBuffer, elementBuffer,
                                                          vertex_layout, n_vertices, format);

        drawable->nIndices = n_indices;
        switch (indices.size() / n_indices) {
            case 1:
                drawable->indicesDataType = GL_UNSIGNED_BYTE;
                break;
            case 2:
                drawable->indicesDataType = GL_UNSIGNED_SHORT;
                break;
            case 4:
                drawable->indicesDataType = GL_UNSIGNED_INT;
                break;
            default:
                throw std::invalid_argument("malformed vertex information");
        }

        return drawable;
    }

    std::unique_ptr<gl::ElementBuffer>
    DrawableCreator::createElementBuffer(const std::span<const unsigned int>& indices)
    {
        return createElementBuffer(std::as_bytes(indices));
    }

    std::unique_ptr<gl::VertexBuffer>
    DrawableCreator::createVertexBuffer(const std::span<const float>& vertices)
    {
        return createVertexBuffer(std::as_bytes(vertices));
    }

    std::unique_ptr<gl::VertexArray>
    DrawableCreator::createVertexArray(
            const std::shared_ptr<gl::VertexBuffer>& vertex_buffer,
            const std::shared_ptr<gl::ElementBuffer>& element_buffer,
            const std::span<const unsigned int>& vertex_layout,
            unsigned int n_vertices,
            gl::VertexFormat format)
    {
        return createVertexArrayInternal(vertex_buffer, element_buffer, vertex_layout, n_vertices, format);
    }

    std::unique_ptr<VertexArray>
    DrawableCreator::createVertexArrayInternal(
            const std::shared_ptr<gl::VertexBuffer>& vertex_buffer,
            const std::shared_ptr<gl::ElementBuffer>& element_buffer,
            const std::span<const unsigned int>& vertex_layout,
            unsigned int n_vertices,
            gl::VertexFormat format)
    {
        auto context = lockContextPtr();
        auto vertex_array = std::unique_ptr<VertexArray>(new VertexArray(context));
        vertex_array->vertexBuffer = std::static_pointer_cast<VertexBuffer>(vertex_buffer);
        vertex_array->elementBuffer = std::static_pointer_cast<ElementBuffer>(element_buffer);

        glGenVertexArrays(1, &vertex_array->vao);

        context->bindVertexArray(vertex_array->vao);
        // force binding so that vao registers it TODO
        glBindBuffer(GL_ARRAY_BUFFER, vertex_array->vertexBuffer->vbo);
        context->bindBuffer(GL_ARRAY_BUFFER, vertex_array->vertexBuffer->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_array->elementBuffer->ebo);
        context->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_array->elementBuffer->ebo);

        // configure vertex attributes
        const unsigned int vertex_size = std::accumulate(vertex_layout.begin(), vertex_layout.end(), 0u);
        switch (format) {
            case gl::VertexFormat::INTERLEAVED: {
                unsigned int attribute_offset = 0;
                for (unsigned int i = 0; i < vertex_layout.size(); i++) {
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(
                            i, vertex_layout[i],
                            GL_FLOAT, GL_FALSE,
                            vertex_size * sizeof(GLfloat),
                            reinterpret_cast<GLvoid*>(attribute_offset * sizeof(GLfloat)));
                    attribute_offset += vertex_layout[i];
                }
                break;
            }
            case gl::VertexFormat::BATCHED: {
                unsigned int attribute_offset = 0;
                for (unsigned int i = 0; i < vertex_layout.size(); i++) {
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(
                            i, vertex_layout[i],
                            GL_FLOAT, GL_FALSE,
                            vertex_layout[i] * sizeof(GLfloat),
                            reinterpret_cast<GLvoid*>(attribute_offset * sizeof(GLfloat)));
                    attribute_offset += vertex_layout[i] * n_vertices;
                }
                break;
            }
            default:
                throw std::invalid_argument("Unsupported vertex format");
        }

        context->bindVertexArray(0);

        vertex_array->id = vertex_array->vao;
        vertex_array->layout.assign(vertex_layout.begin(), vertex_layout.end());
        vertex_array->vertexSize = vertex_size;
        vertex_array->format = format;
        return vertex_array;
    }

    std::unique_ptr<gl::VertexBuffer>
    DrawableCreator::createVertexBuffer(const std::span<const std::byte>& vertices)
    {
        auto context = lockContextPtr();
        auto vertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(context));

        glGenBuffers(1, &vertexBuffer->vbo);

        context->bindBuffer(GL_ARRAY_BUFFER, vertexBuffer->vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), static_cast<GLenum>(vertexBuffer->usage));

        vertexBuffer->id = vertexBuffer->vbo;
        vertexBuffer->byteSize = vertices.size();
        vertexBuffer->empty = vertices.empty();
        return vertexBuffer;
    }

    std::unique_ptr<gl::ElementBuffer>
    DrawableCreator::createElementBuffer(const std::span<const std::byte>& indices)
    {
        auto context = lockContextPtr();
        auto elementBuffer = std::unique_ptr<ElementBuffer>(new ElementBuffer(context));

        // we must unbind any VAO to not accidentally bind the new EBO to an unrelated VAO
        context->bindVertexArray(0);

        glGenBuffers(1, &elementBuffer->ebo);

        context->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);

        elementBuffer->id = elementBuffer->ebo;
        elementBuffer->byteSize = indices.size();
        return elementBuffer;
    }
}

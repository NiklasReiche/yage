#include "DrawableCreator.h"

#include "Context.h"
#include "Drawable.h"
#include "OpenGL.h"
#include <memory>
#include <numeric>

namespace opengl
{
	std::unique_ptr<gl::IDrawable>
	DrawableCreator::createDrawable(
		const std::span<const float>& vertices,
		const std::span<const unsigned int>& indices,
		const std::vector<unsigned int>& vertexLayout,
		gl::VertexFormat format)
	{
		auto context = lockContextPtr();
		auto drawable = std::make_unique<Drawable>();

        const unsigned int vertexSize = std::accumulate(vertexLayout.begin(), vertexLayout.end(), 0u);
        drawable->nVertices = vertexSize == 0 ? 0 : indices.size();
        const auto nVerticesData = vertexSize == 0 ? 0 : vertices.size() / vertexSize;

		const std::shared_ptr<gl::VertexBuffer> vertexBuffer = createVertexBuffer(vertices);
		const std::shared_ptr<gl::ElementBuffer> elementBuffer = createElementBuffer(indices);
		drawable->vertexArray = createVertexArrayInternal(vertexBuffer, elementBuffer, vertexLayout,
                                                          nVerticesData, format);

		return drawable;
	}

	std::unique_ptr<gl::IDrawable> DrawableCreator::createDrawable(
		const std::span<const unsigned char>& vertices,
		const std::span<const unsigned char>& indices,
		const std::vector<unsigned int>& vertexLayout,
		unsigned int nVertices,
		gl::VertexFormat format)
	{
		auto context = lockContextPtr();
		auto drawable = std::make_unique<Drawable>();

		const std::shared_ptr<gl::VertexBuffer> vertexBuffer = createVertexBuffer(vertices);
		const std::shared_ptr<gl::ElementBuffer> elementBuffer = createElementBuffer(indices);
		drawable->vertexArray = createVertexArrayInternal(vertexBuffer, elementBuffer, vertexLayout, nVertices, format);

		drawable->nVertices = nVertices;
		switch (indices.size() / nVertices) {
			case 1: drawable->indicesDataType = GL_UNSIGNED_BYTE;
				break;
			case 2: drawable->indicesDataType = GL_UNSIGNED_SHORT;
				break;
			case 4: drawable->indicesDataType = GL_UNSIGNED_INT;
				break;
			default: throw std::invalid_argument("malformed vertex information");
		}

		return drawable;
	}

	std::unique_ptr<gl::ElementBuffer>
	DrawableCreator::createElementBuffer(const std::span<const unsigned int>& indices)
	{
		return createElementBuffer((const unsigned char*)indices.data(), indices.size() * sizeof(unsigned int));
	}

	std::unique_ptr<gl::ElementBuffer>
	DrawableCreator::createElementBuffer(const std::span<const unsigned char>& indices)
	{
		return createElementBuffer(indices.data(), indices.size());
	}

	std::unique_ptr<gl::VertexBuffer>
	DrawableCreator::createVertexBuffer(const std::span<const float>& vertices)
	{
		return createVertexBuffer((const unsigned char*)vertices.data(), vertices.size() * sizeof(float));
	}

	std::unique_ptr<gl::VertexBuffer>
	DrawableCreator::createVertexBuffer(const std::span<const unsigned char>& vertices)
	{
		return createVertexBuffer(vertices.data(), vertices.size());
	}

	std::unique_ptr<gl::VertexArray>
	DrawableCreator::createVertexArray(
		const std::shared_ptr<gl::VertexBuffer>& vertexBuffer,
		const std::shared_ptr<gl::ElementBuffer>& elementBuffer,
		const std::vector<unsigned int>& vertexLayout,
        unsigned int nVertices,
		gl::VertexFormat format)
	{
		return createVertexArrayInternal(vertexBuffer, elementBuffer, vertexLayout, nVertices, format);
	}

	std::unique_ptr<VertexArray>
	DrawableCreator::createVertexArrayInternal(
		const std::shared_ptr<gl::VertexBuffer>& vertexBuffer,
		const std::shared_ptr<gl::ElementBuffer>& elementBuffer,
		const std::vector<unsigned int>& vertexLayout,
        unsigned int nVertices,
		gl::VertexFormat format)
	{
		auto context = lockContextPtr();
		auto vertexArray = std::unique_ptr<VertexArray>(new VertexArray(context));
		vertexArray->vertexBuffer = std::static_pointer_cast<VertexBuffer>(vertexBuffer);
		vertexArray->elementBuffer = std::static_pointer_cast<ElementBuffer>(elementBuffer);

		glGenVertexArrays(1, &vertexArray->vao);

		context->bindVertexArray(vertexArray->vao);
		// force binding so that vao registers it TODO
		glBindBuffer(GL_ARRAY_BUFFER, vertexArray->vertexBuffer->vbo);
		context->bindBuffer(GL_ARRAY_BUFFER, vertexArray->vertexBuffer->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray->elementBuffer->ebo);
		context->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray->elementBuffer->ebo);

		// configure vertex attributes
		const unsigned int vertexSize = std::accumulate(vertexLayout.begin(), vertexLayout.end(), 0u);
		switch (format) {
			case gl::VertexFormat::INTERLEAVED: {
				unsigned int attributeOffset = 0;
				for (unsigned int i = 0; i < vertexLayout.size(); i++) {
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(
						i, vertexLayout[i],
						GL_FLOAT, GL_FALSE,
						vertexSize * sizeof(GLfloat),
						reinterpret_cast<GLvoid*>(attributeOffset * sizeof(GLfloat)));
					attributeOffset += vertexLayout[i];
				}
				break;
			}
			case gl::VertexFormat::BATCHED: {
                unsigned int attributeOffset = 0;
                for (unsigned int i = 0; i < vertexLayout.size(); i++) {
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(
                        i, vertexLayout[i],
                        GL_FLOAT, GL_FALSE,
                        vertexLayout[i] * sizeof(GLfloat),
                        reinterpret_cast<GLvoid*>(attributeOffset * sizeof(GLfloat)));
                    attributeOffset += vertexLayout[i] * nVertices;
                }
                break;
			}

			default: throw std::invalid_argument("Unsupported vertex format");
		}

		context->bindVertexArray(0);

		vertexArray->id = vertexArray->vao;
		vertexArray->layout = vertexLayout;
		vertexArray->vertexSize = vertexSize;
		vertexArray->format = format;
		return vertexArray;
	}

	std::unique_ptr<gl::VertexBuffer>
	DrawableCreator::createVertexBuffer(const unsigned char* vertices, std::size_t size)
	{
		auto context = lockContextPtr();
		auto vertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(context));

		glGenBuffers(1, &vertexBuffer->vbo);

		context->bindBuffer(GL_ARRAY_BUFFER, vertexBuffer->vbo);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, static_cast<GLenum>(vertexBuffer->usage));

		vertexBuffer->id = vertexBuffer->vbo;
		vertexBuffer->byteSize = size;
		vertexBuffer->empty = vertices == nullptr;
		return vertexBuffer;
	}

	std::unique_ptr<gl::ElementBuffer>
	DrawableCreator::createElementBuffer(const unsigned char* indices, std::size_t size)
	{
		auto context = lockContextPtr();
		auto elementBuffer = std::unique_ptr<ElementBuffer>(new ElementBuffer(context));

        // we must unbind any VAO to not accidentally bind the new EBO to an unrelated VAO
        context->bindVertexArray(0);

		glGenBuffers(1, &elementBuffer->ebo);

		context->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

		elementBuffer->id = elementBuffer->ebo;
		elementBuffer->byteSize = size;
		return elementBuffer;
	}
}

#include "VertexBuffer.h"
#include "Context.h"

namespace opengl
{
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &vbo);
        lockContextPtr()->bindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		: OpenGlObject(std::move(other))
	{
		*this = std::move(other);
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		if (this != &other) {
			vbo = other.vbo;
			byteSize = other.byteSize;
			empty = other.empty;

			other.vbo = 0;
			other.byteSize = 0;
			other.empty = true;

			OpenGlObject::operator=(std::move(other));
		}
		return *this;
	}

	void VertexBuffer::setData(const std::span<const float>& vertices)
	{
		const auto ptr = lockContextPtr();

		ptr->bindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
		             vertices.size() * sizeof(GLfloat),
		             vertices.empty() ? nullptr : vertices.data(),
		             static_cast<GLenum>(usage));

		this->byteSize = vertices.size();
		this->empty = vertices.empty();
	}

	void VertexBuffer::setSubData(unsigned int offset, const std::span<const float>& vertices)
	{
		if (offset + vertices.size() > byteSize)
			throw std::invalid_argument("Trying to write more elements than available");

		if (vertices.empty())
			return;

		lockContextPtr()->bindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferSubData(GL_ARRAY_BUFFER,
		                offset * sizeof(GLfloat),
		                vertices.size() * sizeof(GLfloat),
		                vertices.data());

		empty = false;
	}

	std::vector<float> VertexBuffer::getSubData(const unsigned int offset, const unsigned int size)
	{
		if (offset + size > byteSize)
			throw std::invalid_argument("Trying to read more elements than available");

		if (size == 0)
			return std::vector<float>();

		lockContextPtr()->bindBuffer(GL_ARRAY_BUFFER, vbo);

		auto data = std::vector<float>(size);
		glGetBufferSubData(GL_ARRAY_BUFFER,
		                   offset * sizeof(GLfloat),
		                   size * sizeof(GLfloat),
		                   data.data());

		return data;
	}

	bool opengl::VertexBuffer::isEmpty()
	{
		return empty;
	}
}

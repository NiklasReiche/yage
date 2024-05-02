#include "VertexArray.h"

namespace opengl
{
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &vao);
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept
		: OpenGlObject(std::move(other))
	{
		*this = std::move(other);
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		if (*this != other) {
			vao = other.vao;
			vertexBuffer = std::move(other.vertexBuffer);
			elementBuffer = std::move(other.elementBuffer);
			layout = other.layout;
			vertexSize = other.vertexSize;
			format = other.format;

			other.vao = 0;
			other.layout = { };
			other.vertexSize = 0;
			other.format = format;
		}
		return *this;
	}

    void VertexArray::setSubData(unsigned int offset, const std::vector<float>& vertices)
    {
        vertexBuffer->setSubData(offset, vertices);
    }
}

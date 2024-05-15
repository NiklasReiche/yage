#include "ElementBuffer.h"
#include "Context.h"

namespace opengl
{
	ElementBuffer::~ElementBuffer()
	{
		glDeleteBuffers(1, &ebo);
        lockContextPtr()->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept
		: OpenGlObject(std::move(other))
	{
		*this = std::move(other);
	}

	ElementBuffer& ElementBuffer::operator=(ElementBuffer&& other) noexcept
	{
		if (this != &other) {
			ebo = other.ebo;
			byteSize = other.byteSize;

			other.ebo = 0;
			other.byteSize = 0;

			OpenGlObject::operator=(std::move(other));
		}
		return *this;
	}
}

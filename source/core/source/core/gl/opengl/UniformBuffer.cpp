#include "UniformBuffer.h"
#include "Context.h"

namespace opengl
{
	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &ubo);
        lockContextPtr()->bindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
		: OpenGlObject(std::move(other))
	{
		this->ubo = other.ubo;

		other.ubo = 0;
	}

	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other)
	noexcept
	{
		if (this != &other) {
			OpenGlObject::operator=(std::move(other));

			this->ubo = other.ubo;

			other.ubo = 0;
		}

		return *this;
	}

	void UniformBuffer::setData(const void* data, const std::size_t size)
	{
		lockContextPtr()->bindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void UniformBuffer::setSubData(const std::size_t offset, const void* data, const std::size_t size)
	{
		lockContextPtr()->bindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	std::string UniformBuffer::getName() const
	{
		return name;
	}
}

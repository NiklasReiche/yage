#include "Drawable.h"
#include "Context.h"

namespace opengl
{
	Drawable::~Drawable()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	Drawable::Drawable(Drawable&& other) noexcept
		: OpenGlObject(std::move(other))
	{
		*this = std::move(other);
	}

	Drawable& Drawable::operator=(Drawable&& other) noexcept
	{
		if (this != &other)
		{
			VAO = other.VAO;
			VBO = other.VBO;
			primitive = other.primitive;
			usage = other.usage;
			nVertices = other.nVertices;
			layout = std::move(other.layout);
			vertexSize = other.vertexSize;
			format = other.format;
			bufferSize = other.bufferSize;

			other.VAO = 0;
			other.VBO = 0;
			other.primitive = PrimitiveType();
			other.usage = DrawMode();
			other.nVertices = 0;
			other.vertexSize = 0;
			other.format = gl::VertexFormat();
			other.bufferSize = 0;

			OpenGlObject::operator=(std::move(other));
		}
		return *this;
	}

	void Drawable::setData(const std::span<const float>& vertices)
	{
		const auto ptr = lockContextPtr();
		
		ptr->bindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(
			GL_ARRAY_BUFFER, 
			vertices.size() * sizeof(GLfloat), 
			vertices.empty() ? nullptr : vertices.data(), 
			static_cast<GLenum>(usage));
		
		this->nVertices = static_cast<int>(vertices.size()) / vertexSize;
		this->bufferSize = nVertices * vertexSize;
		this->empty = vertices.empty();

		if (format == gl::VertexFormat::BATCHED) 
		{
			ptr->bindVertexArray(VAO);
			unsigned int attributeOffset = 0;
			for (unsigned int i = 0; i < layout.size(); i++) 
			{
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(
					i, layout[i], 
					GL_FLOAT, GL_FALSE, 
					layout[i] * sizeof(GLfloat),
					reinterpret_cast<GLvoid*>(attributeOffset * sizeof(GLfloat)));
				attributeOffset += layout[i] * nVertices;
			}
		}
	}

	void Drawable::setSubData(unsigned int offset, const std::span<const float>& vertices)
	{
		if (offset + vertices.size() > bufferSize)
			throw std::invalid_argument("Trying to write more elements than available");

		if (vertices.empty())
			return;
		
		lockContextPtr()->bindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferSubData(
			GL_ARRAY_BUFFER, 
			offset * sizeof(GLfloat), 
			vertices.size() * sizeof(GLfloat), 
			vertices.data());

		empty = false;
	}

	std::vector<float> Drawable::getSubData(const unsigned int offset, const unsigned int size)
	{
		if (offset + size > bufferSize)
			throw std::invalid_argument("Trying to read more elements than available");
		
		if (size == 0)
			return std::vector<float>();
		
		lockContextPtr()->bindBuffer(GL_ARRAY_BUFFER, VBO);
		
		auto data = std::vector<float>(size);
		glGetBufferSubData(
			GL_ARRAY_BUFFER, 
			offset * sizeof(GLfloat),
			size * sizeof(GLfloat), 
			data.data());
		
		return data;
	}

	bool Drawable::isEmpty()
	{
		return empty;
	}
}

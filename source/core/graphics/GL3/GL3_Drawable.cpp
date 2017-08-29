#include "GL3_Drawable.h"
#include "GL3_Context.h"

namespace gl3
{
	GL3_Drawable::GL3_Drawable()
		: GL3_Object() {}
	GL3_Drawable::GL3_Drawable(const GL3_Drawable& other)
		: GL3_Object(other)
	{
		this->VAO = other.VAO;
		this->VBO = other.VBO;
		this->primitive = other.primitive;
		this->usage = other.usage;
		this->nVertices = other.nVertices;
		this->layout = other.layout;
		this->vertexSize = other.vertexSize;
		this->format = other.format;
	}
	GL3_Drawable::~GL3_Drawable()
	{
		if (*refCount == 1) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}
	}
	GL3_Drawable& GL3_Drawable::operator=(const GL3_Drawable& other)
	{
		GL3_Object::operator=(other);
		if (shouldDelete) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			shouldDelete = false;
		}

		this->VAO = other.VAO;
		this->VBO = other.VBO;
		this->primitive = other.primitive;
		this->usage = other.usage;
		this->nVertices = other.nVertices;
		this->layout = other.layout;
		this->vertexSize = other.vertexSize;
		this->format = other.format;

		return *this;
	}

	void GL3_Drawable::bufferSubData(int offset, std::vector<GLfloat> & vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), &vertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GL3_Drawable::bufferData(std::vector<GLfloat> & vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], (GLenum)usage);
		
		this->nVertices = (int)vertices.size() / vertexSize;

		if (format == VertexFormat::BATCHED) {
			glBindVertexArray(VAO);
			int attribOffset = 0;
			for (unsigned int i = 0; i < layout.size(); i++) {
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, layout[i], GL_FLOAT, GL_FALSE, layout[i] * sizeof(GLfloat), (GLvoid*)(attribOffset * sizeof(GLfloat)));
				attribOffset += layout[i] * nVertices;
			}
			glBindVertexArray(0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
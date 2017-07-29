#include "GL3_Drawable.h"
#include "GL3_Context.h"

namespace gl3
{
	GL3_Drawable::GL3_Drawable()
		: GL3_Object() {}
	GL3_Drawable::~GL3_Drawable()
	{
		if (*refCount == 1) {
			glDeleteBuffers(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}
	}
	GL3_Drawable::GL3_Drawable(const GL3_Drawable& copy)
		: GL3_Object(copy)
	{
		this->VAO = copy.VAO;
		this->VBO = copy.VBO;
		this->primitive = copy.primitive;
		this->usage = copy.usage;
		this->nVertices = copy.nVertices;
		this->layout = copy.layout;
		this->vertexSize = copy.vertexSize;
		this->format = copy.format;
	}
	GL3_Drawable& GL3_Drawable::operator=(const GL3_Drawable& copy)
	{
		GL3_Object::operator=(copy);
		if (shouldDelete) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			shouldDelete = false;
		}

		this->VAO = copy.VAO;
		this->VBO = copy.VBO;
		this->primitive = copy.primitive;
		this->usage = copy.usage;
		this->nVertices = copy.nVertices;
		this->layout = copy.layout;
		this->vertexSize = copy.vertexSize;
		this->format = copy.format;

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
		
		int _nVertices = (int)vertices.size() / vertexSize;
		this->nVertices = _nVertices;

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
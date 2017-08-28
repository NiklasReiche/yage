#include "GLES2_Drawable.h"
#include "GLES2_Context.h"

namespace gles2
{
	GLES2_Drawable::GLES2_Drawable()
		: GLES2_Object() {}
	GLES2_Drawable::GLES2_Drawable(const GLES2_Drawable& other)
		: GLES2_Object(other)
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
	GLES2_Drawable::~GLES2_Drawable()
	{
		if (*refCount == 1) {
			glDeleteBuffers(1, &VBO);
		}
	}
	GLES2_Drawable& GLES2_Drawable::operator=(const GLES2_Drawable& other)
	{
		GLES2_Object::operator=(other);
		if (shouldDelete) {
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

	void GLES2_Drawable::bufferSubData(int offset, std::vector<GLfloat> & vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), &vertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLES2_Drawable::bufferData(std::vector<GLfloat> & vertices) 
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], (GLenum)usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->nVertices = (int)vertices.size() / vertexSize;

		if (format == VertexFormat::BATCHED) {
			VertexArrayObject vao;
			int attribOffset = 0;
			for (unsigned int i = 0; i < layout.size(); i++) {
				VertexAttrib attrib = { (GLint)i, (GLint)layout[i], GL_FLOAT, GL_FALSE, (GLsizei)(layout[i] * sizeof(GLfloat)), (int)(attribOffset * sizeof(GLfloat)) };
				vao.attributes.push_back(attrib);
				attribOffset += layout[i] * nVertices;
			}
			this->VAO = vao;
		}
	}
}
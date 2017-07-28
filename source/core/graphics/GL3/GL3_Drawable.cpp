#include "GL3_Drawable.h"
#include "GL3_Context.h"

namespace gl3
{
	void GL3_Drawable::bufferSubData(int offset, std::vector<GLfloat> & vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), &vertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GL3_Drawable::bufferData(std::vector<GLfloat> & vertices, int nVertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], (GLenum)usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		this->nVertices = nVertices;
	}
}
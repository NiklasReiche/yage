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
}
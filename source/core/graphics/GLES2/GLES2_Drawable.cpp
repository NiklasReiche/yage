#include "GLES2_Drawable.h"
#include "GLES2_Context.h"

namespace gles2
{
	void GLES2_Drawable::bufferSubData(int offset, std::vector<GLfloat> & vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), &vertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
#pragma once

#include <vector>

#include "gles2.h"
#include "GLES2_Object.h"
#include "GLES2_Enum.h"

namespace gles2
{
	struct VertexAttrib
	{
		GLint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		int pointer;
	};
	struct VertexArrayObject
	{
		std::vector<VertexAttrib> attributes;
	};

	class GLES2_Drawable : public GLES2_Object
	{
	private:
		friend class GLES2_Context;

		GLuint VBO;
		VertexArrayObject VAO;
		GLint nVertices;
		PrimitiveType primitive;
		DrawMode usage;

	public:
		void bufferSubData(int offset, std::vector<GLfloat> & vertices);
	};
}
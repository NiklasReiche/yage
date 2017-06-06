#pragma once

#include <vector>

#include "gl3.h"
#include "GL3_Object.h"
#include "GL3_Enum.h"

namespace gl3
{
	class GL3_Drawable : public GL3_Object
	{
	private:
		friend class GL3_Context;

		GLuint VAO;
		GLuint VBO;
		GLint nVertices;
		PrimitiveType primitive;
		DrawMode usage;

	public:
		void bufferSubData(int offset, std::vector<GLfloat> & vertices);
	};
}
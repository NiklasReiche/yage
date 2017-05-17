#pragma once

#include "gl3.h"
#include "GL3_Enum.h"

namespace gl3
{
	struct GL3_Drawable
	{
		GLuint VAO;
		GLuint VBO;
		GLint nVertices;
		PrimitiveType primitive;
		DrawMode usage;
	};
}
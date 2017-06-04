#pragma once

namespace gles2
{
	struct Drawable
	{
		GLuint VBO;
		GLint nVertices;
		PrimitiveType primitive;
	};
}
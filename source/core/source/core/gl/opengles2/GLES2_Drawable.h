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

		GLuint VBO = 0;
		VertexArrayObject VAO;
		PrimitiveType primitive;
		DrawMode usage;
		GLint nVertices;
		std::vector<int> layout;
		int vertexSize;
		VertexFormat format;

	public:
		GLES2_Drawable();
		GLES2_Drawable(const GLES2_Drawable& other);
		virtual ~GLES2_Drawable();
		GLES2_Drawable& operator=(const GLES2_Drawable& other);

		void bufferSubData(int offset, std::vector<GLfloat> & vertices);
		void bufferData(std::vector<GLfloat> & vertices);
	};
}
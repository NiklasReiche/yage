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

		GLuint VAO = 0;
		GLuint VBO = 0;
		PrimitiveType primitive;
		DrawMode usage;
		GLint nVertices;
		std::vector<int> layout;
		int vertexSize;
		VertexFormat format;

	public:
		GL3_Drawable();
		GL3_Drawable(const GL3_Drawable& other);
		virtual ~GL3_Drawable();
		GL3_Drawable& operator=(const GL3_Drawable& other);

		void bufferSubData(int offset, std::vector<GLfloat> & vertices);
		void bufferData(std::vector<GLfloat> & vertices);
	};
}
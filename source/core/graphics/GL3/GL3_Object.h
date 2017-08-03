#pragma once

#include "gl3.h"

namespace gl3
{
	class GL3_Context;

	class GL3_Object
	{
	private:
		friend class GL3_Context;

	protected:
		int* refCount = nullptr;
		bool shouldDelete = false;

		GLuint id = 0;
		GL3_Context* glContext = nullptr;

	public:
		GL3_Object();
		GL3_Object(const GL3_Object& copy);
		virtual ~GL3_Object();
		GL3_Object& operator=(const GL3_Object& copy);

		friend bool operator==(const GL3_Object & left, const GL3_Object & right);
		friend bool operator!=(const GL3_Object & left, const GL3_Object & right);
	};
}
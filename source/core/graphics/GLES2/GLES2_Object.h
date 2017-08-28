#pragma once

#include "gles2.h"

namespace gles2
{
	class GLES2_Context;

	class GLES2_Object
	{
	private:
		friend class GLES2_Context;

	protected:
		int* refCount = nullptr;
		bool shouldDelete = false;

		GLuint id = 0;
		GLES2_Context* glContext = nullptr;

	public:
		GLES2_Object();
		GLES2_Object(const GLES2_Object& copy);
		virtual ~GLES2_Object();
		GLES2_Object& operator=(const GLES2_Object& copy);

		friend bool operator==(const GLES2_Object & left, const GLES2_Object & right);
		friend bool operator!=(const GLES2_Object & left, const GLES2_Object & right);
	};
}
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
		GLuint id;
		GLES2_Context* glContext;

	public:
		friend bool operator==(const GLES2_Object & left, const GLES2_Object & right);
		friend bool operator!=(const GLES2_Object & left, const GLES2_Object & right);
	};

	inline bool operator==(const GLES2_Object & left, const GLES2_Object & right)
	{
		return left.id == right.id;
	}
	inline bool operator!=(const GLES2_Object & left, const GLES2_Object & right)
	{
		return !(left.id == right.id);
	}
}
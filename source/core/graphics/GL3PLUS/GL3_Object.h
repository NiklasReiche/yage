#pragma once

#include "gl3.h"
//#include "GL3_Context.h"

namespace gl3
{
	//class GL3_Context;

	class GL3_Object
	{
		friend class GL3_Context;
	protected:
		GLuint id;
		
	public:
		friend bool operator==(const GL3_Object & left, const GL3_Object & right);
		friend bool operator!=(const GL3_Object & left, const GL3_Object & right);
	};

	inline bool operator==(const GL3_Object & left, const GL3_Object & right)
	{
		return left.id == right.id;
	}
	inline bool operator!=(const GL3_Object & left, const GL3_Object & right)
	{
		return !(left.id == right.id);
	}
}
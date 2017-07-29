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

		GLuint id;
		GL3_Context* glContext;

	public:
		GL3_Object()
		{
			refCount = new int(1);
		}
		GL3_Object(const GL3_Object& copy)
		{
			this->id = copy.id;
			this->glContext = copy.glContext;
			this->refCount = copy.refCount;
			++(*refCount);
		}
		virtual ~GL3_Object()
		{
			if (*refCount == 1) {
				delete refCount;
			}
			else {
				--(*refCount);
			}
		}

		GL3_Object& operator=(const GL3_Object& copy) 
		{
			if (*refCount == 1) {
				delete refCount;
				shouldDelete = true;
			}
			else {
				--(*refCount);
			}

			this->id = copy.id;
			this->glContext = copy.glContext;
			this->refCount = copy.refCount;
			++(*refCount);

			return *this;
		}

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
#include "GL3_Object.h"

namespace gl3
{
	GL3_Object::GL3_Object()
	{
		refCount = new int(1);
	}
	GL3_Object::GL3_Object(const GL3_Object& copy)
	{
		this->id = copy.id;
		this->glContext = copy.glContext;
		this->refCount = copy.refCount;
		++(*refCount);
	}
	GL3_Object::~GL3_Object()
	{
		if (*refCount == 1) {
			delete refCount;
		}
		else {
			--(*refCount);
		}
	}
	GL3_Object& GL3_Object::operator=(const GL3_Object& copy)
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


	bool operator==(const GL3_Object & left, const GL3_Object & right)
	{
		return left.id == right.id;
	}
	bool operator!=(const GL3_Object & left, const GL3_Object & right)
	{
		return !(left.id == right.id);
	}
}
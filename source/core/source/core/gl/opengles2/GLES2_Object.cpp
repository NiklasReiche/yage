#include "GLES2_Object.h"

namespace gles2
{
	GLES2_Object::GLES2_Object()
	{
		refCount = new int(1);
	}
	GLES2_Object::GLES2_Object(const GLES2_Object& copy)
	{
		this->id = copy.id;
		this->glContext = copy.glContext;
		this->refCount = copy.refCount;
		++(*refCount);
	}
	GLES2_Object::~GLES2_Object()
	{
		if (*refCount == 1) {
			delete refCount;
		}
		else {
			--(*refCount);
		}
	}
	GLES2_Object& GLES2_Object::operator=(const GLES2_Object& copy)
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


	bool operator==(const GLES2_Object & left, const GLES2_Object & right)
	{
		return left.id == right.id;
	}
	bool operator!=(const GLES2_Object & left, const GLES2_Object & right)
	{
		return !(left.id == right.id);
	}
}
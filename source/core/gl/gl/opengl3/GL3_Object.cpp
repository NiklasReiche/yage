#include "GL3_Object.h"

namespace gl3
{
	bool operator==(const GL3_Object & left, const GL3_Object & right)
	{
		return left.id == right.id;
	}
	
	bool operator!=(const GL3_Object & left, const GL3_Object & right)
	{
		return !(left == right);
	}

	bool GL3_Object::isValid() const
	{
		return id > 0;
	}

	GLuint GL3_Object::getId() const
	{
		return id;
	}

	GL3_Object::GL3_Object(GL3_Object&& other) noexcept
		: GL3_BaseObject(std::move(other))
	{
		*this = std::move(other);
	}

	GL3_Object& GL3_Object::operator=(GL3_Object&& other) noexcept
	{
		if (*this != other)
		{
			this->id = other.id;
			other.id = 0;

			GL3_BaseObject::operator=(std::move(other));
		}
		return *this;
	}
}

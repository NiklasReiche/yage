#include "OpenGlObject.h"

namespace opengl
{
	bool operator==(const OpenGlObject& left, const OpenGlObject& right)
	{
		return left.id == right.id;
	}
	
	bool operator!=(const OpenGlObject& left, const OpenGlObject& right)
	{
		return !(left == right);
	}

	bool OpenGlObject::isValid() const
	{
		return id > 0;
	}

	GLuint OpenGlObject::getId() const
	{
		return id;
	}

	OpenGlObject::OpenGlObject(OpenGlObject&& other) noexcept
		: BaseObject(std::move(other))
	{
		*this = std::move(other);
	}

	OpenGlObject& OpenGlObject::operator=(OpenGlObject&& other) noexcept
	{
		if (*this != other)
		{
			this->id = other.id;
			other.id = 0;

			BaseObject::operator=(std::move(other));
		}
		return *this;
	}
}

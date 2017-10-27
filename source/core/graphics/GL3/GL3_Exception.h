#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

#include "gl3.h"
#include <platform/exception.h>

namespace gl3
{
	enum GlErrorCodes {
		GLFW_ERROR,
		GLAD_ERROR
	};

	class GlException : public sys::Exception
	{
	public:
		GlException(int code, std::string msg = "")
			: Exception(code, msg) {}
	};
}

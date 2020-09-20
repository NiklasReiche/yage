#pragma once

#include <stdexcept>
#include <string>

namespace sys::desktop
{
	class GlfwException final : public std::runtime_error
	{
	public:
		explicit GlfwException(const int code, const std::string& msg = "")
			: std::runtime_error(msg), code(code) {}

		[[nodiscard]]
		int error() const
		{
			return code;
		}

	private:
		int code;
	};
}

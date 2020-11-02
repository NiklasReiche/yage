#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace platform
{
	class FileException final : public std::runtime_error
	{
	public:
		enum ErrorCode
		{
			UNKNOWN,
			FILE_NOT_FOUND,
			ACCESS_VIOLATION,
			BAD_FILE,
			BAD_IO
		};
		
		explicit FileException(const ErrorCode code, const std::string& msg = "", std::string filename = "")
			: std::runtime_error(msg), code(code), filename(std::move(filename)) {}

		[[nodiscard]]
		ErrorCode error() const
		{
			return code;
		}
		
		[[nodiscard]]
		std::string file() const
		{
			return filename;
		}

	private:
		ErrorCode code;
		std::string filename;
	};
}

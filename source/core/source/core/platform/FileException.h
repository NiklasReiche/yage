#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace yage::platform
{
	class FileException : public std::runtime_error
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
			: std::runtime_error(msg), code(code), filename(std::move(filename))
		{ }

		explicit FileException(std::string filename = "")
			: std::runtime_error(""), code(ErrorCode::UNKNOWN), filename(std::move(filename))
		{ }

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

	class FileOpenException : public FileException
	{
	public:
		explicit FileOpenException(std::string filename)
			: FileException(std::move(filename))
		{ }

		explicit FileOpenException(const ErrorCode code, const std::string& msg = "", std::string filename = "")
			: FileException(code, msg, std::move(filename))
		{ }
	};

	class FileSeekException : public FileException
	{
	public:
		explicit FileSeekException(std::string filename)
			: FileException(std::move(filename))
		{ }

		explicit FileSeekException(const ErrorCode code, const std::string& msg = "", std::string filename = "")
			: FileException(code, msg, std::move(filename))
		{ }
	};

	class FileReadException : public FileException
	{
	public:
		explicit FileReadException(const std::string& msg, const std::string& filename)
			: FileException(ErrorCode::UNKNOWN, msg, filename)
		{ }

		explicit FileReadException(const ErrorCode code, const std::string& msg = "", std::string filename = "")
			: FileException(code, msg, std::move(filename))
		{ }
	};

	class FileWriteException : public FileException
	{
	public:
		explicit FileWriteException(const ErrorCode code, const std::string& msg = "", std::string filename = "")
			: FileException(code, msg, filename)
		{ }
	};
}

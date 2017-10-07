#pragma once

#include <string>
#include <exception>

namespace sys
{
	class Exception : public std::exception
	{
	private:
		int code;
		std::string msg;

	public:
		Exception(int code, std::string msg = "")
			: code(code), msg(msg) {}

		int error() { return code; }
		std::string what() { return msg; }
	};

	enum FileError
	{
		UNKNOWN,
		FILE_NOT_FOUND,
		ACCESS_VIOLATION,
		BAD_FILE,
		BAD_IO
	};
	class FileException : public Exception
	{
	private:
		std::string filename;

	public:
		FileException(int code, std::string msg = "", std::string filename = "")
			: Exception(code, msg), filename(filename) {}

		std::string file() { return filename; }
	};
}
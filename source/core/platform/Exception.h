#pragma once

#include <string>
#include <exception>

enum class FileError
{
	UNKNOWN,
	PATH_VIOLATION,
	ACCESS_VIOLATION
};

class FileException : public std::exception
{
private:
	std::string filename;
	FileError error;

public:
	FileException(FileError error, std::string path = "")
		: filename(path), error(error) {}
	
	FileError what() { return error; }
	std::string path() { return filename; }
};
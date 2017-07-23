#pragma once

#include <string>
#include <exception>

class FileException : public std::exception
{
private:
	std::string path;

public:
	FileException(std::string path)
		: path(path) {}
	
	std::string what() { return path; }
};
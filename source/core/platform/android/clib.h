#pragma once

#include <sstream>
#include <string>

namespace clib
{
	template<typename T>
	std::string to_string(T param)
	{
		std::stringstream sstream;
		sstream << param;
		return sstream.str();
	}
}
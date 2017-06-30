#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace clib
{
	template<typename T>
	inline std::string to_string(T param)
	{
		std::stringstream sstream;
		sstream << param;
		return sstream.str();
	}

	inline void strip(std::string string, std::string delimiter, std::vector<std::string> & out)
	{
		out.resize(0);

		size_t pos = 0;
		std::string token;
		while ((pos = string.find(delimiter)) != std::string::npos) {
			token = string.substr(0, pos);
			string.erase(0, pos + delimiter.length());
			out.push_back(token);
		}
	}
	inline std::vector<std::string> strip(std::string string, std::string delimiter)
	{
		std::vector<std::string> out;
		strip(string, delimiter, out);
		return out;
	}
}
#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace utils
{
	template<typename T>
	std::string toString(T param)
	{
		std::stringstream stream;
		stream << param;
		return stream.str();
	}

	inline std::vector<std::string> strip(std::string string, const std::string& delimiter)
	{
		std::vector<std::string> out;

		size_t pos;
		while ((pos = string.find(delimiter)) != std::string::npos) {
			std::string token = string.substr(0, pos);
			string.erase(0, pos + delimiter.length());
			out.push_back(token);
		}
		out.push_back(string);

		return out;
	}

	inline std::string join(const std::vector<std::string>& strings)
	{
		std::stringstream stream;
		for (const auto& s : strings) {
			stream << s;
		}
		return stream.str();
	}

	inline std::string& replaceAll(std::string& s, const std::string& find, const std::string& replace)
	{
		size_t pos;
		while ((pos = s.find(find)) != std::string::npos) {
			s.replace(pos, find.length(), replace);
		}
		return s;
	}

	inline bool contains(const std::string& input, const std::string& search)
	{
		return input.find(search) != std::string::npos;
	}

	inline bool startsWith(const std::string& input, const std::string& prefix)
	{
		return input.rfind(prefix, 0) == 0;
	}
}
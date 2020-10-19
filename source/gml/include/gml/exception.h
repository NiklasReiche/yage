#pragma once

#include <exception>

namespace gml
{
	class DivideByZeroException : public std::exception
	{
	};

	class InvalidDimensionException : public std::exception
	{
	};
}

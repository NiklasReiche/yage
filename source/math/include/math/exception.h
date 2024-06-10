#pragma once

#include <exception>

namespace yage::math
{
	class DivideByZeroException final : public std::exception
	{
	};

	class InvalidDimensionException final : public std::exception
	{
	};
}

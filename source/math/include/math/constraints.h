#pragma once

#include <cstddef>

namespace yage::math
{
	template<std::size_t N>
	concept StrictlyPositive = N > 0;
}

#pragma once

#include <cstddef>

namespace gml
{
	template<std::size_t N>
	concept StrictlyPositive = N > 0;
}

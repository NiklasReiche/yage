#pragma once

#include "MVector.h"

namespace gml
{
	template<typename T>
	inline T lerp(T start, T end, float delta)
	{
		return start + (end - start) * delta;
	}
}
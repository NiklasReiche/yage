#pragma once

namespace gml
{
	template<typename T>
	inline T lerp(T start, T end, double delta)
	{
		return static_cast<T>(start + (end - start) * delta);
	}
}
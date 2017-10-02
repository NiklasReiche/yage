#pragma once

namespace gml
{
	template<typename T>
	inline T lerp(T start, T end, float delta)
	{
		return (T)(start + (end - start) * delta);
	}
}
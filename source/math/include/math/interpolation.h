#pragma once

namespace yage::math
{
	/**
	 * Linearly interpolates between two points.
	 *
	 * @tparam T The type of the points.
	 * @param start The start point of the interpolation.
	 * @param end The end point of the interpolation.
	 * @param delta The interpolant clamped to [0,1].
	 * @return The interpolated value between start and end.
	 */
	template<typename T>
	T lerp(const T& start, const T& end, double delta)
	{
		if (delta <= 0)
			return start;
		if (delta >= 1)
			return end;
		return static_cast<T>(start + (end - start) * delta);
	}
}

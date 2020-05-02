#pragma once

#include <cmath>

namespace gml
{
	// TODO: Replace with c++20 <numbers> implementation
	const double PI = 3.141592653589793238463;

	/**
	 * @brief Converts degrees to radians.
	 * @param deg A value in degrees.
	 * @return A value in radians.
	 */
	inline double toRad(const double deg)
	{
		return deg * (PI / 180.0);
	}

	/**
	 * @copydoc toRad(double)
	 */
	inline float toRad(const float deg)
	{
		return deg * (static_cast<float>(PI) / 180.0f);
	}

	/**
	 * @brief Converts radians to degrees.
	 * @param rad A value in radians.
	 * @return A value in degrees.
	 */
	inline double toDeg(const double rad)
	{
		return rad * (180.0 / PI);
	}

	/**
	 * @copydoc toDeg(double)
	 */
	inline float toDeg(const float rad)
	{
		return rad * (180.0f / static_cast<float>(PI));
	}
}
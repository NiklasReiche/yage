#pragma once

#include <cmath>
#include <numbers>

namespace gml
{
	/**
	 * @brief Converts degrees to radians.
	 * @param deg A value in degrees.
	 * @return A value in radians.
	 */
	inline double toRad(const double deg)
	{
		return deg * (std::numbers::pi / 180.0);
	}

	/**
	 * @copydoc toRad(double)
	 */
	inline float toRad(const float deg)
	{
		return deg * (std::numbers::pi_v<float> / 180.0f);
	}

	/**
	 * @brief Converts radians to degrees.
	 * @param rad A value in radians.
	 * @return A value in degrees.
	 */
	inline double toDeg(const double rad)
	{
		return rad * (180.0 / std::numbers::pi);
	}

	/**
	 * @copydoc toDeg(double)
	 */
	inline float toDeg(const float rad)
	{
		return rad * (180.0f / std::numbers::pi_v<float>);
	}
}
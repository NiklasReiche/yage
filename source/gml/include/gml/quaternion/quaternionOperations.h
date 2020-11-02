#pragma once

#include "quaternion.h"

namespace gml
{
	/**
	 * @brief Calculates the squared length of a quaternion.
	 *
	 * @tparam T The quaternion's data type.
	 * @param q The quaternion of which to calculate the length.
	 * @return The square of the quaternion's length.
	 */
	template<typename T>
	constexpr double sqrLength(const Quaternion<T>& q)
	{
		return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	}

	/**
	 * @brief Calculates the length of a quaternion.
	 *
	 * @tparam T The quaternion's data type.
	 * @param q The quaternion of which to calculate the length.
	 * @return The quaternion's length.
	 */
	template<typename T>
	constexpr double length(const Quaternion<T>& q)
	{
		return sqrt(sqrLength(q));
	}

	/**
	 * @brief Normalizes a quaternion.
	 *
	 * @tparam T The quaternion's data type.
	 * @param q the quaternion to normalize.
	 * @return A normalized copy of the quaternion.
	 */
	template<typename T>
	constexpr Quaternion<T> normalize(Quaternion<T> q)
	{
		return q.normalize();
	}

	/**
	 * @brief Returns the conjugate of a quaternion.
	 *
	 * @tparam T The quaternion's data type.
	 * @param q The quaternion to conjugate.
	 * @return A conjugate copy of the quaternion.
	 */
	template<typename T>
	Quaternion<T> conjugate(Quaternion<T> q)
	{
		return q.conjugate();
	}
}

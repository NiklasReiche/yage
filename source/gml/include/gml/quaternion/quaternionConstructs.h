#pragma once

#include "quaternion.h"

namespace gml::quaternion
{
	/**
	 * @brief Constructs a quaternion from an axis angle representation.
	 *
	 * @param axis the rotation axis
	 * @param angle the angle in radians
	 * @return the quaternion
	 */
	template<typename T>
	constexpr Quaternion<T> axisAngle(const Vec3<T>& axis, const double angle)
	{
		const gml::Vec3<T> a = gml::normalize(axis);
		const double s = std::sin(angle / 2);

		return Quaternion<T>(std::cos(angle / 2), a.x * s, a.y * s, a.z * s);
	}

	/**
	 * @brief Constructs a quaternion from an euler angle representation.
	 *
	 * @param yaw the rotation around the y axis in radians
	 * @param roll the rotation around the z axis in radians
	 * @param pitch the rotation around the x axis in radians
	 * @return the quaternion
	 */
	template<typename T>
	constexpr Quaternion<T> eulerAngle(const double yaw, const double roll, const double pitch)
	{
		const double cy = std::cos(yaw / 2);
		const double cr = std::cos(roll / 2);
		const double cp = std::cos(pitch / 2);
		const double sy = std::sin(yaw / 2);
		const double sr = std::sin(roll / 2);
		const double sp = std::sin(pitch / 2);

		const double w = cy * cr * cp - sy * sr * sp;
		const double x = sy * sr * cp + cy * cr * sp;
		const double y = sy * cr * cp + cy * sr * sp;
		const double z = cy * sr * cp - sy * cr * sp;
		return Quaternion<T>(w, x, y, z);
	}

	/**
	 * @brief Constructs a quaternion from a rotation matrix.
	 *
	 * @param matrix the rotation matrix
	 * @return the quaternion
	 */
	template<typename T>
	constexpr Quaternion<T> rotationMatrix(Mat3<T> matrix)
	{
		const double tr = matrix.trace();
		Mat3<T>& m = matrix;
		Quaternion<T> q;
		if (tr > 0) {
			const double s = std::sqrt(tr + 1) * 2;
			q.w = 0.25 * s;
			q.x = (m.at(2, 1) - m.at(1, 2)) / s;
			q.y = (m.at(0, 2) - m.at(2, 0)) / s;
			q.z = (m.at(1, 0) - m.at(0, 1)) / s;
		} else if ((m.at(0, 0) > m.at(1, 1)) && (m.at(0, 0) > m.at(2, 2))) {
			const double s = std::sqrt(1.0 + m.at(0, 0) - m.at(1, 1) - m.at(2, 2)) * 2;
			q.w = (m.at(2, 1) - m.at(1, 2)) / s;
			q.x = 0.25 * s;
			q.y = (m.at(0, 1) + m.at(1, 0)) / s;
			q.z = (m.at(0, 2) + m.at(2, 0)) / s;
		} else if (m.at(1, 1) > m.at(2, 2)) {
			const double s = std::sqrt(1.0 + m.at(1, 1) - m.at(0, 0) - m.at(2, 2)) * 2;
			q.w = (m.at(0, 2) - m.at(2, 0)) / s;
			q.x = (m.at(0, 1) + m.at(1, 0)) / s;
			q.y = 0.25 * s;
			q.z = (m.at(1, 2) + m.at(2, 1)) / s;
		} else {
			const double s = std::sqrt(1.0 + m.at(2, 2) - m.at(0, 0) - m.at(1, 1)) * 2;
			q.w = (m.at(1, 0) - m.at(0, 1)) / s;
			q.x = (m.at(0, 2) + m.at(2, 0)) / s;
			q.y = (m.at(1, 2) + m.at(2, 1)) / s;
			q.z = 0.25 * s;
		}
		return q;
	}
}

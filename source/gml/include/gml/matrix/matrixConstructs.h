#pragma once

#include "mat4.h"

namespace gml::matrix
{
	/**
	 * @brief Constructs a translation matrix.
	 *
	 * @param x the x component
	 * @param y the y component
	 * @param z the z component
	 * @return the translation matrix
	*/
	template<typename T>
	Mat4<T> translate(const T& x, const T& y, const T& z)
	{
		Mat4<T> result = Mat4<T>::Id;
		result(0, 3) = x;
		result(1, 3) = y;
		result(2, 3) = z;
		return result;
	}

	/**
	 * @brief Constructs a translation matrix from a vector.
	 *
	 * @param translation the translation vector
	 * @return the translation matrix
	 */
	template<typename T>
	Mat4<T> translate(const Vec3<T>& translation)
	{
		return translate(translation(0), translation(1), translation(2));
	}

	/**
	 * @brief Constructs a scaling matrix.
	 *
	 * @param x the x component
	 * @param y the y component
	 * @param z the z component
	 * @return the scaling matrix
	 */
	template<typename T>
	Mat4<T> scale(const T& x, const T& y, const T& z)
	{
		Mat4<T> result = Mat4<T>::Id;
		result(0, 0) = x;
		result(1, 1) = y;
		result(2, 2) = z;
		return result;
	}

	/**
	 * @brief Constructs a scaling matrix from a vector.
	 *
	 * @param scale the scaling vector
	 * @return the scaling matrix
	 */
	template<typename T>
	Mat4<T> scale(const Vec3<T>& scale)
	{
		return Mat4<T>::scale(scale.at(0), scale.at(1), scale.at(2));
	}

	/**
	 * @brief Constructs a rotation matrix from an axis angle
	 * representation.
	 *
	 * @param axis the rotation axis
	 * @param angle the angle in radians
	 * @return the rotation matrix
	 */
	template<typename T>
	Mat4<T> axisAngle(const Vec3<T>& axis, const double angle)
	{
		Vec3<T> a = gml::normalize(axis);
		const T x = a.x;
		const T y = a.y;
		const T z = a.z;
		const T c = std::cos(angle);
		const T s = std::sin(angle);
		const T t = 1 - c;

		Mat4<T> result = Mat4<T>::Id;
		result(0, 0) = x * x * t + c;
		result(0, 1) = x * y * t - z * s;
		result(0, 2) = x * z * t + y * s;

		result(1, 0) = y * x * t + z * s;
		result(1, 1) = y * y * t + c;
		result(1, 2) = y * z * t - x * s;

		result(2, 0) = z * x * t - y * s;
		result(2, 1) = z * y * t + x * s;
		result(2, 2) = z * z * t + c;
		return result;
	}

	/**
	 * @brief Constructs a rotation matrix from a quaternion
	 * representation.
	 *
	 * @return the rotation matrix
	 */
	template<typename T>
	Mat4<T> quaternion(const Quaternion<T>& quaternion)
	{
		const Quaternion<T> q = normalize(quaternion);

		Mat4<T> result = Mat4<T>::Id;
		result(0, 0) = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
		result(0, 1) = 2 * q.x * q.y - 2 * q.z * q.w;
		result(0, 2) = 2 * q.x * q.z + 2 * q.y * q.w;

		result(1, 0) = 2 * q.x * q.y + 2 * q.z * q.w;
		result(1, 1) = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
		result(1, 2) = 2 * q.y * q.z - 2 * q.x * q.w;

		result(2, 0) = 2 * q.x * q.z - 2 * q.y * q.w;
		result(2, 1) = 2 * q.y * q.z + 2 * q.x * q.w;
		result(2, 2) = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
		return result;
	}

	/**
	 * @brief Constructs a perspective projection matrix.
	 *
	 * @param fov the field of view in degrees
	 * @param aspect the aspect ration
	 * @param near the near plane
	 * @param far the far plane
	 * @return the projection matrix
	 */
	template<typename T>
	Mat4<T> perspective(const double fov, const double aspect, const double near, const double far)
	{
		Mat4<T> result = Mat4<T>::Id;
		const double top = near * std::tan(toRad(fov / 2));
		const double bottom = 0 - top;
		const double right = top * aspect;
		const double left = 0 - right;
		result(0, 0) = (2 * near) / (right - left);
		result(0, 2) = (right + left) / (right - left);
		result(1, 1) = (2 * near) / (top - bottom);
		result(1, 2) = (top + bottom) / (top - bottom);
		result(2, 2) = -((far + near) / (far - near));
		result(2, 3) = -((2 * far * near) / (far - near));
		result(3, 2) = -1;
		result(3, 3) = 0;
		return result;
	}

	/**
	 * @brief Constructs an orthographic projection matrix.
	 *
	 * @param left the left plane
	 * @param right the right plane
	 * @param bottom the bottom plane
	 * @param top the top plane
	 * @param near the near plane
	 * @param far the far plane
	 * @return the projection matrix
	 */
	template<typename T>
	Mat4<T> orthographic(
		const double left, const double right,
		const double bottom, const double top,
		const double near, const double far)
	{
		Mat4<T> result = Mat4<T>::Id;
		result(0, 0) = 2 / (right - left);
		result(0, 3) = -((right + left) / (right - left));
		result(1, 1) = 2 / (top - bottom);
		result(1, 3) = -((top + bottom) / (top - bottom));
		result(2, 2) = -2 / (far - near);
		result(2, 3) = -((far + near) / (far - near));
		result(3, 3) = 1;
		return result;
	}

	/**
	 * @brief Constructs a lookAt view matrix.
	 *
	 * @param pos the camera position
	 * @param target the view target position
	 * @param up the camera up vector
	 * @return the view matrix
	 */
	template<typename T>
	Mat4<T> lookAt(const Vec3<T>& pos, const Vec3<T>& target, const Vec3<T>& up)
	{
		Vec3<T> direction = normalize(pos - target);
		Vec3<T> right = normalize(cross(up, direction));
		Mat4<T> mat1 = {
			{ right.x,     right.y,     right.z,     0 },
			{ up.x,        up.y,        up.z,        0 },
			{ direction.x, direction.y, direction.z, 0 },
			{ 0,           0,           0,           1 }
		};
		return mat1 * translate<T>(-pos.x, -pos.y, -pos.z);
	}
}

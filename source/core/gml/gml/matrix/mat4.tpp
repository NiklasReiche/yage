#pragma once

namespace gml
{
	template<typename T>
	Mat4 <T> Mat4<T>::translate(const T& x, const T& y, const T& z)
	{
		Mat4<T> result = Id;
		result(0, 3) = x;
		result(1, 3) = y;
		result(2, 3) = z;
		return result;
	}

	template<typename T>
	Mat4 <T> Mat4<T>::translate(const Vec3 <T>& translation)
	{
		return Mat4<T>::translate(translation(0), translation(1), translation(2));
	}

	template<typename T>
	Mat4 <T> Mat4<T>::scale(const T& x, const T& y, const T& z)
	{
		Mat4<T> result = Id;
		result(0, 0) = x;
		result(1, 1) = y;
		result(2, 2) = z;
		return result;
	}

	template<typename T>
	Mat4 <T> Mat4<T>::scale(const Vec3 <T>& scale)
	{
		return Mat4<T>::scale(scale.at(0), scale.at(1), scale.at(2));
	}

	template<typename T>
	Mat4 <T> Mat4<T>::axisAngle(const Vec3 <T>& axis, const double angle)
	{
		Vec3<T> a = gml::normalize(axis);
		const T x = a.x;
		const T y = a.y;
		const T z = a.z;
		const T c = std::cos(angle);
		const T s = std::sin(angle);
		const T t = 1 - c;

		Mat4<T> result;
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

	template<typename T>
	Mat4 <T> Mat4<T>::quaternion(const Quaternion <T>& quaternion)
	{
		const Quaternion<T> q = normalize(quaternion);;

		Mat4<T> result;
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

	template<typename T>
	Mat4 <T> Mat4<T>::perspective(
		const double fov, const double aspect,
		const double near, const double far)
	{
		Mat4<T> result;
		const double top = near * std::tan(toRad(fov / 2));
		const double bottom = 0 - top;
		const double right = top * aspect;
		const double left = 0 - right;
		result(0, 0) = (2 * near) / (right - left);
		result(0, 2) = (right + left) / (right - left);
		result(1, 1) = (2 * near) / (top - bottom);
		result(1, 2) = (top + bottom) / (top - bottom);
		result(2, 2) = 0 - ((far + near) / (far - near));
		result(2, 3) = 0 - ((2 * far * near) / (far - near));
		result(3, 2) = -1;
		result(3, 3) = 0;
		return result;
	}

	template<typename T>
	Mat4 <T> Mat4<T>::orthographic(
		const double left, const double right,
		const double bottom, const double top,
		const double near, const double far)
	{
		Mat4<T> result;
		result(0, 0) = 2 / (right - left);
		result(0, 3) = -((right + left) / (right - left));
		result(1, 1) = 2 / (top - bottom);
		result(1, 3) = -((top + bottom) / (top - bottom));
		result(2, 2) = -2 / (far - near);
		result(2, 3) = -((far + near) / (far - near));
		result(3, 3) = 1;
		return result;
	}

	template<typename T>
	Mat4 <T> Mat4<T>::lookAt(
		const Vec3 <T>& pos,
		const Vec3 <T>& target,
		const Vec3 <T>& up)
	{
		Vec3<T> direction = normalize(pos - target);
		Vec3<T> right = normalize(cross(up, direction));
		Mat4<T> mat1 = {
			{ right.x,     right.y,     right.z,     0 },
			{ up.x,        up.y,        up.z,        0 },
			{ direction.x, direction.y, direction.z, 0 },
			{ 0,           0,           0,           1 }
		};
		return mat1 * Mat4<T>::translate(-pos.x, -pos.y, -pos.z);
	}
}

#pragma once
#include <iostream>

#include "vector.h"

namespace gml
{
	template <typename T>
	class Quaternion
	{
	public:
		T w, x, y, z;

		Quaternion() :
			w(0), x(0), y(0), z(0) {}
		Quaternion(T w, T x, T y, T z) :
			w(w), x(x), y(y), z(z) {}
		Quaternion(Vec3<T> vec) :
			w(0), x(vec.x), y(vec.y), z(vec.z) {}

		Vec3<T> getForward()
		{
			return normalize(Vec3<T>(-2 * (x * z + w * y), -2 * (y * z - w * x), -1 + 2 * (x * x + y * y)));
		}
		Vec3<T> getRight()
		{
			return normalize(Vec3<T>(1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)));
		}
		Vec3<T> getUp()
		{
			return normalize(Vec3<T>(2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)));
		}

		float getPitch()
		{
			float sinr = 2.0f * (w * x + y * z);
			float cosr = 1.0f - 2.0f * (x * x + y * y);
			return std::atan2(sinr, cosr);
		}
		float getYaw()
		{
			float sinp = 2.0 * (w * y - z * x);
			if (std::abs(sinp) >= 1) {
				return std::copysign(PI / 2, sinp);
			}
			else {
				return std::asin(sinp);
			}
		}
		float getRoll()
		{
			double siny = 2.0 * (w * z + x * y);
			double cosy = 1.0 - 2.0 * (y * y + z * z);
			return std::atan2(siny, cosy);
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, Quaternion<T> vec) {
		os << vec.w << " " << vec.x << " " << vec.y << " " << vec.z;
		return os;
	}

	/*  Quaternion Calculations */
	template <typename T>
	Quaternion<T> operator*(Quaternion<T> A, Quaternion<T> B) {
		Quaternion<T> out;
		Vec3<T> a = Vec3<T>(A.x, A.y, A.z);
		Vec3<T> b = Vec3<T>(B.x, B.y, B.z);
		out.w = A.w * B.w - dot(a, b);
		Vec3<T> c = Vec3<T>(A.w * b) + Vec3<T>(B.w * a) + cross(a, b);
		out.x = c.x;
		out.y = c.y;
		out.z = c.z;
		return out;
	}
	template <typename T>
	Quaternion<T> operator*(Quaternion<T> quat, float s) {
		Quaternion<T> out;
		out.x = quat.x * s;
		out.y = quat.y * s;
		out.z = quat.z * s;
		out.w = quat.w * s;
		return out;
	}
	template <typename T>
	Quaternion<T> operator*(float s, Quaternion<T> quat) {
		Quaternion<T> out;
		out.x = s * quat.x;
		out.y = s * quat.y;
		out.z = s * quat.z;
		out.w = s * quat.w;
		return out;
	}

	template <typename T>
	float length(Quaternion<T> quat) {
		return sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	}
	template <typename T>
	Quaternion<T> normalize(Quaternion<T> quat) {
		if (length(quat) == 0) {
			return quat;
		}
		return (1 / length(quat)) * quat;
	}
	template <typename T>
	Quaternion<T> conjugate(Quaternion<T> quat) {
		Quaternion<T> out;
		out.w = quat.w;
		out.x = -quat.x;
		out.y = -quat.y;
		out.z = -quat.z;
		return out;
	}

	template <typename T>
	Quaternion<T> axisAngle(float angle, Vec3<T> axis) {
		Quaternion<T> q;
		float rad = angle * (PI / 180);
		float s = sin(rad / 2);
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		q.w = std::cos(rad / 2);
		return q;
	}
	template <typename T>
	Quaternion<T> eulerAngle(float yaw, float pitch, float roll) {
		//Quaternion qroll = Quaternion(std::cos(angle_y / 2), sin(angle_y / 2), 0, 0);
		//Quaternion qpitch = Quaternion(std::cos(angle_x / 2), 0, sin(angle_x / 2), 0);
		//Quaternion qyaw = Quaternion(std::cos(angle_z / 2), 0, 0, sin(angle_z / 2));
		return Quaternion<T>();
	}
}
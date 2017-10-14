#pragma once
#include <iostream>

#include "math.h"

namespace gml
{
	template <typename T>
	class Vec2;
	template <typename T>
	class Vec3;
	template <typename T>
	class Vec4;

	typedef Vec2<int> Vec2i;
	typedef Vec3<int> Vec3i;
	typedef Vec4<int> Vec4i;

	typedef Vec2<unsigned int> Vec2ui;
	typedef Vec3<unsigned int> Vec3ui;
	typedef Vec4<unsigned int> Vec4ui;

	typedef Vec2<float> Vec2f;
	typedef Vec3<float> Vec3f;
	typedef Vec4<float> Vec4f;

	typedef Vec2<double> Vec2d;
	typedef Vec3<double> Vec3d;
	typedef Vec4<double> Vec4d;


	template <typename T>
	class Vec2
	{
	public:
		T x, y;

		Vec2() :
			x(0), y(0) {}
		Vec2(T s) :
			x(s), y(s) {}
		Vec2(T x, T y) :
			x(x), y(y) {}

		Vec2& operator+=(const Vec2& right)
		{
			this->x += right.x;
			this->y += right.y;
			return *this;
		}
		Vec2& operator-=(const Vec2& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			return *this;
		}
		Vec2& operator*=(float right)
		{
			this->x *= right;
			this->y *= right;
			return *this;
		}
		Vec2& operator/=(float right)
		{
			this->x /= right;
			this->y /= right;
			return *this;
		}
	};

	template <typename T>
	class Vec3
	{
	public:
		T x, y, z;

		Vec3() :
			x(0), y(0), z(0) {}
		Vec3(T s) :
			x(s), y(s), z(s) {}
		Vec3(T x, T y, T z) :
			x(x), y(y), z(z) {}

		Vec3& operator+=(const Vec3& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			return *this;
		}
		Vec3& operator-=(const Vec3& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			return *this;
		}
		Vec3& operator*=(float right)
		{
			this->x *= right;
			this->y *= right;
			this->z *= right;
			return *this;
		}
		Vec3& operator/=(float right)
		{
			this->x /= right;
			this->y /= right;
			this->z /= right;
			return *this;
		}
	};

	template <typename T>
	class Vec4
	{
	public:
		T x, y, z, w;

		Vec4() :
			x(0), y(0), z(0), w(0) {}
		Vec4(T s) :
			x(s), y(s), z(s), w(s) {}
		Vec4(T x, T y, T z, T w) :
			x(x), y(y), z(z), w(w) {}

		Vec4& operator+=(const Vec4& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			this->w += right.w;
			return *this;
		}
		Vec4& operator-=(const Vec4& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			this->w -= right.w;
			return *this;
		}
		Vec4& operator*=(float right)
		{
			this->x *= right;
			this->y *= right;
			this->z *= right;
			this->w *= right;
			return *this;
		}
		Vec4& operator/=(float right)
		{
			this->x /= right;
			this->y /= right;
			this->z /= right;
			this->w /= right;
			return *this;
		}

		Vec3<T> toVec3()
		{
			Vec3<T> vec;
			vec.x = this->x * this->w;
			vec.y = this->y * this->w;
			vec.z = this->z * this->w;
			return vec;
		}
	};

	/*  Output Stream */
	template <typename T>
	std::ostream& operator<<(std::ostream& os, Vec2<T> vec) {
		os << vec.x << " " << vec.y;
		return os;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, Vec3<T> vec) {
		os << vec.x << " " << vec.y << " " << vec.z;
		return os;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, Vec4<T> vec) {
		os << vec.x << " " << vec.y << " " << vec.z << " " << vec.w;
		return os;
	}

	/*  Comparision */
	template <typename T>
	bool operator==(const Vec2<T>& left, const Vec2<T>& right) {
		return left.x == right.x && left.y == right.y;
	}
	template <typename T>
	bool operator==(const Vec3<T>& left, const Vec3<T>& right) {
		return left.x == right.x && left.y == right.y && left.z == right.z;
	}
	template <typename T>
	bool operator==(const Vec4<T>& left, const Vec4<T>& right) {
		return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
	}

	template <typename T>
	bool operator!=(const Vec2<T>& left, const Vec2<T>& right) {
		return !(left == right);
	}
	template <typename T>
	bool operator!=(const Vec3<T>& left, const Vec3<T>& right) {
		return !(left == right);
	}
	template <typename T>
	bool operator!=(const Vec4<T>& left, const Vec4<T>& right) {
		return !(left == right);
	}

	/*  Addition */
	template <typename T>
	Vec2<T> operator+(Vec2<T> left, Vec2<T> right) {
		Vec2<T> out;
		out.x = left.x + right.x;
		out.y = left.y + right.y;
		return out;
	}
	template <typename T>
	Vec3<T> operator+(Vec3<T> left, Vec3<T> right) {
		Vec3<T> out;
		out.x = left.x + right.x;
		out.y = left.y + right.y;
		out.z = left.z + right.z;
		return out;
	}
	template <typename T>
	Vec4<T> operator+(Vec4<T> left, Vec4<T> right) {
		Vec4<T> out;
		out.x = left.x + right.x;
		out.y = left.y + right.y;
		out.z = left.z + right.z;
		out.w = left.w + right.w;
		return out;
	}

	/*  Subtraction */
	template <typename T>
	Vec2<T> operator-(Vec2<T> left, Vec2<T> right) {
		Vec2<T> out;
		out.x = left.x - right.x;
		out.y = left.y - right.y;
		return out;
	}
	template <typename T>
	Vec3<T> operator-(Vec3<T> left, Vec3<T> right) {
		Vec3<T> out;
		out.x = left.x - right.x;
		out.y = left.y - right.y;
		out.z = left.z - right.z;
		return out;
	}
	template <typename T>
	Vec4<T> operator-(Vec4<T> left, Vec4<T> right) {
		Vec4<T> out;
		out.x = left.x - right.x;
		out.y = left.y - right.y;
		out.z = left.z - right.z;
		out.w = left.w - right.w;
		return out;
	}

	/*  Multiplication */
	template <typename T>
	Vec2<T> operator*(Vec2<T> left, Vec2<T> right) {
		Vec2<T> out;
		out.x = left.x * right.x;
		out.y = left.y * right.y;
		return out;
	}
	template <typename T>
	Vec2<T> operator*(Vec2<T> vec, float s) {
		Vec2<T> out;
		out.x = vec.x * s;
		out.y = vec.y * s;
		return out;
	}
	template <typename T>
	Vec2<T> operator*(float s, Vec2<T> vec) {
		Vec2<T> out;
		out.x = s * vec.x;
		out.y = s * vec.y;
		return out;
	}

	template <typename T>
	Vec3<T> operator*(Vec3<T> left, Vec3<T> right) {
		Vec3<T> out;
		out.x = left.x * right.x;
		out.y = left.y * right.y;
		out.z = left.z * right.z;
		return out;
	}
	template <typename T>
	Vec3<T> operator*(Vec3<T> vec, float s) {
		Vec3<T> out;
		out.x = vec.x * s;
		out.y = vec.y * s;
		out.z = vec.z * s;
		return out;
	}
	template <typename T>
	Vec3<T> operator*(float s, Vec3<T> vec) {
		Vec3<T> out;
		out.x = s * vec.x;
		out.y = s * vec.y;
		out.z = s * vec.z;
		return out;
	}

	template <typename T>
	Vec4<T> operator*(Vec4<T> left, Vec4<T> right) {
		Vec4<T> out;
		out.x = left.x * right.x;
		out.y = left.y * right.y;
		out.z = left.z * right.z;
		out.w = left.w * right.w;
		return out;
	}
	template <typename T>
	Vec4<T> operator*(Vec4<T> vec, float s) {
		Vec4<T> out;
		out.x = vec.x * s;
		out.y = vec.y * s;
		out.z = vec.z * s;
		out.w = vec.w * s;
		return out;
	}
	template <typename T>
	Vec4<T> operator*(float s, Vec4<T> vec) {
		Vec4<T> out;
		out.x = s * vec.x;
		out.y = s * vec.y;
		out.z = s * vec.z;
		out.w = s * vec.w;
		return out;
	}

	/*  Division */
	template <typename T>
	Vec2<T> operator/(Vec2<T> vec, float s) {
		Vec2<T> out;
		out.x = vec.x / s;
		out.y = vec.y / s;
		return out;
	}
	template <typename T>
	Vec3<T> operator/(Vec3<T> vec, float s) {
		Vec3<T> out;
		out.x = vec.x / s;
		out.y = vec.y / s;
		out.z = vec.z / s;
		return out;
	}
	template <typename T>
	Vec4<T> operator/(Vec4<T> vec, float s) {
		Vec4<T> out;
		out.x = vec.x / s;
		out.y = vec.y / s;
		out.z = vec.z / s;
		out.w = vec.w / s;
		return out;
	}

	/*  Vector Calculations */
	template <typename T>
	Vec3<T> cross(Vec3<T> left, Vec3<T> right) {
		Vec3<T> out;
		out.x = left.y * right.z - left.z * right.y;
		out.y = left.z * right.x - left.x * right.z;
		out.z = left.x * right.y - left.y * right.x;
		return out;
	}
	template <typename T>
	T dot(Vec3<T> left, Vec3<T> right) {
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	template <typename T>
	double length(Vec2<T> vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	template <typename T>
	double length(Vec3<T> vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
	template <typename T>
	double length(Vec4<T> vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	}

	template <typename T>
	Vec2<T> normalize(Vec2<T> vec) {
		if (length(vec) == 0) {
			return vec;
		}
		return (1 / length(vec)) * vec;
	}
	template <typename T>
	Vec3<T> normalize(Vec3<T> vec) {
		if (length(vec) == 0) {
			return vec;
		}
		return (1 / length(vec)) * vec;
	}
	template <typename T>
	Vec4<T> normalize(Vec4<T> vec) {
		if (length(vec) == 0) {
			return vec;
		}
		return (1 / length(vec)) * vec;
	}
}
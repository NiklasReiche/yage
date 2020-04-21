#pragma once
#include "matrix.h"

namespace gml
{
	template <typename T>
	Quaternion<T>::Quaternion()
		: w(1), x(0), y(0), z(0)
	{
	}

	template <typename T>
	Quaternion<T>::Quaternion(const T& w, const T& x, const T& y, const T& z)
		: w(w), x(x), y(y), z(z)
	{
	}

	template <typename T>
	Quaternion<T>::Quaternion(const Vec3<T> & vector)
		: w(0), x(vector.x), y(vector.y), z(vector.z)
	{
	}

	template <typename T>
	double Quaternion<T>::sqrLength() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	double Quaternion<T>::length() const
	{
		return sqrt(sqrLength());
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::normalize()
	{
		const auto len = length();
		if (len == 0)
		{
			throw DivideByZeroException();
		}
		return operator*=(1 / len);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::conjugate()
	{
		this->x *= -1;
		this->y *= -1;
		this->z *= -1;
		return *this;
	}

	template <typename T>
	Vec3<T> Quaternion<T>::getForward() const
	{
		Quaternion<T> q = gml::normalize(*this);
		Quaternion<T> result = q * Quaternion<T>(Vec3<T>::worldForward()) * gml::conjugate(q);
		return gml::normalize(Vec3<T>(result.x, result.y, result.z));
	}

	template <typename T>
	Vec3<T> Quaternion<T>::getRight() const
	{
		Quaternion<T> q = gml::normalize(*this);
		Quaternion<T> result = q * Quaternion<T>(Vec3<T>::worldRight()) * gml::conjugate(q);
		return gml::normalize(Vec3<T>(result.x, result.y, result.z));
	}

	template <typename T>
	Vec3<T> Quaternion<T>::getUp() const
	{
		Quaternion<T> q = gml::normalize(*this);
		Quaternion<T> result = q * Quaternion<T>(Vec3<T>::worldUp()) * gml::conjugate(q);
		return gml::normalize(Vec3<T>(result.x, result.y, result.z));
	}

	template <typename T>
	double Quaternion<T>::getPitch() const
	{
		const auto unit = sqrLength();
		const double test = x * y + z * w;
		if (test > 0.499*unit || test < -0.499*unit)
		{
			return 0; // singularity at north and south pole
		}
		const double sqw = w * w;
		const double sqx = x * x;
		const double sqy = y * y;
		const double sqz = z * z;
		return std::atan2(2 * x * w - 2 * y * z, -sqx + sqy - sqz + sqw);
	}

	template <typename T>
	double Quaternion<T>::getYaw() const
	{
		const auto unit = sqrLength();
		const double test = x * y + z * w;
		if (test > 0.499*unit)
		{
			return 2 * std::atan2(x, w); // singularity at north pole
		}
		if (test < -0.499*unit)
		{
			return -2 * std::atan2(x, w); // singularity at south pole
		}
		const double sqw = w * w;
		const double sqx = x * x;
		const double sqy = y * y;
		const double sqz = z * z;
		return std::atan2(2 * y * w - 2 * x * z, sqx - sqy - sqz + sqw);
	}

	template <typename T>
	double Quaternion<T>::getRoll() const
	{
		const auto unit = sqrLength();
		const double test = x * y + z * w;
		if (test > 0.499*unit)
		{
			return PI / 2; // singularity at north pole
		}
		if (test < -0.499*unit)
		{
			return -PI / 2; // singularity at south pole
		}
		return std::asin(2 * test / unit);
	}

	template <typename T>
	Mat4<T> Quaternion<T>::toMatrix()
	{
		return Mat4<T>::quaternion(*this);
	}

	template <typename T>
	template <typename T2>
	Quaternion<T>& Quaternion<T>::operator*=(const T2& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		this->w *= rhs;
		return *this;
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& rhs)
	{
		Vec3<T> a = Vec3<T>(this->x, this->y, this->z);
		Vec3<T> b = Vec3<T>(rhs.x, rhs.y, rhs.z);

		Vec3<T> c = this->w * b + rhs.w * a + cross(a, b);
		T w = this->w * rhs.w - dot(a, b);

		this->w = w;
		this->x = c.x;
		this->y = c.y;
		this->z = c.z;
		return *this;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::axisAngle(const Vec3<T>& axis, double angle)
	{
		gml::Vec3<T> a = gml::normalize(axis);
		const double s = std::sin(angle / 2);

		Quaternion<T> q;
		q.w = std::cos(angle / 2);
		q.x = a.x * s;
		q.y = a.y * s;
		q.z = a.z * s;
		return q;
	}

	template <typename T>
	Quaternion<T> Quaternion<T>::eulerAngle(
		const double yaw, 
		const double roll,
		const double pitch)
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

	template <typename T>
	Quaternion<T> Quaternion<T>::rotationMatrix(Mat3<T> matrix)
	{
		const double tr = matrix.trace();
		Mat3<T> & m = matrix;
		Quaternion<T> q;
		if (tr > 0)
		{
			const double s = std::sqrt(tr + 1) * 2;
			q.w = 0.25 * s;
			q.x = (m.at(2, 1) - m.at(1, 2)) / s;
			q.y = (m.at(0, 2) - m.at(2, 0)) / s;
			q.z = (m.at(1, 0) - m.at(0, 1)) / s;
		}
		else if ((m.at(0, 0) > m.at(1, 1)) && (m.at(0, 0) > m.at(2, 2)))
		{
			const double s = std::sqrt(1.0 + m.at(0, 0) - m.at(1, 1) - m.at(2, 2)) * 2;
			q.w = (m.at(2, 1) - m.at(1, 2)) / s;
			q.x = 0.25 * s;
			q.y = (m.at(0, 1) + m.at(1, 0)) / s;
			q.z = (m.at(0, 2) + m.at(2, 0)) / s;
		}
		else if (m.at(1, 1) > m.at(2, 2))
		{
			const double s = std::sqrt(1.0 + m.at(1, 1) - m.at(0, 0) - m.at(2, 2)) * 2;
			q.w = (m.at(0, 2) - m.at(2, 0)) / s;
			q.x = (m.at(0, 1) + m.at(1, 0)) / s;
			q.y = 0.25 * s;
			q.z = (m.at(1, 2) + m.at(2, 1)) / s;
		}
		else 
		{
			const double s = std::sqrt(1.0 + m.at(2, 2) - m.at(0, 0) - m.at(1, 1)) * 2;
			q.w = (m.at(1, 0) - m.at(0, 1)) / s;
			q.x = (m.at(0, 2) + m.at(2, 0)) / s;
			q.y = (m.at(1, 2) + m.at(2, 1)) / s;
			q.z = 0.25 * s;
		}
		return q;
	}

	template <typename T>
	Quaternion<T> normalize(const Quaternion<T>& quaternion)
	{
		return Quaternion<T>(quaternion).normalize();
	}

	template <typename T>
	Quaternion<T> conjugate(const Quaternion<T>& quaternion)
	{
		return Quaternion<T>(quaternion).conjugate();
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Quaternion<T>& rhs)
	{
		os << rhs.w << " " << rhs.x << " " << rhs.y << " " << rhs.z;
		return os;
	}

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		return Quaternion<T>(lhs) *= rhs;
	}

	template <typename T, typename T2>
	Quaternion<T> operator*(const Quaternion<T>& lhs, const T2& rhs)
	{
		return Quaternion<T>(lhs) *= rhs;;
	}

	template <typename T, typename T2>
	Quaternion<T> operator*(const T2& lhs, const Quaternion<T>& rhs)
	{
		return Quaternion<T>(rhs) *= lhs;
	}
}

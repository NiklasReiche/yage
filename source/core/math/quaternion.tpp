#pragma once

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
	double Quaternion<T>::length() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
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
		return normalize(Vec3<T>{
			-2 * (x * z + w * y),
			-2 * (y * z - w * x),
			-1 + 2 * (x * x + y * y)
		});
	}

	template <typename T>
	Vec3<T> Quaternion<T>::getRight() const
	{
		return normalize(Vec3<T>{
			1 - 2 * (y * y + z * z),
			2 * (x * y + w * z),
			2 * (x * z - w * y)
		});
	}

	template <typename T>
	Vec3<T> Quaternion<T>::getUp() const
	{
		return normalize(Vec3<T>{
			2 * (x * y - w * z),
			1 - 2 * (x * x + z * z),
			2 * (y * z + w * x)
		});
	}

	template <typename T>
	double Quaternion<T>::getPitch() const
	{
		float test = x * y + z * w;
		if (test > 0.499)
		{
			// singularity at north pole
			return 0;
		}
		if (test < -0.499)
		{
			// singularity at south pole
			return 0;
		}
		float sqx = x * x;
		float sqz = z * z;
		return std::atan2(2 * x * w - 2 * y * z, 1 - 2 * sqx - 2 * sqz);
	}

	template <typename T>
	double Quaternion<T>::getYaw() const
	{
		float test = x * y + z * w;
		if (test > 0.499)
		{
			// singularity at north pole
			return 2 * std::atan2(x, w);
		}
		if (test < -0.499)
		{
			// singularity at south pole
			return -2 * std::atan2(x, w);
		}
		float sqy = y * y;
		float sqz = z * z;
		return std::atan2(2 * y * w - 2 * x * z, 1 - 2 * sqy - 2 * sqz);
	}

	template <typename T>
	double Quaternion<T>::getRoll() const
	{
		float test = x * y + z * w;
		if (test > 0.499)
		{
			// singularity at north pole
			return PI / 2;
		}
		if (test < -0.499)
		{
			// singularity at south pole
			return -PI / 2;
		}
		return std::asin(2 * test);
	}

	template <typename T>
	Quaternion<T>& Quaternion<T>::operator*=(const T& rhs)
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
	static Quaternion<T> axisAngle(double angle, const Vec3<T>& axis)
	{
		axis.normalze();
		angle = toRad(angle);
		const double s = std::sin(angle / 2);

		Quaternion<T> q;
		q.w = std::cos(angle / 2);
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		return q;
	}

	template <typename T>
	static Quaternion<T> eulerAngle(
		const double pitch, 
		const double yaw, 
		const double roll)
	{
		const double c1 = std::cos(toRad(yaw) / 2);
		const double c2 = std::cos(toRad(roll) / 2);
		const double c3 = std::cos(toRad(pitch) / 2);
		const double s1 = std::sin(toRad(yaw) / 2);
		const double s2 = std::sin(toRad(roll) / 2);
		const double s3 = std::sin(toRad(pitch) / 2);
		const double w = c1 * c2 * c3 - s1 * s2 * s3;
		const double x = s1 * s2 * c3 + c1 * c2 * s3;
		const double y = s1 * c2 * c3 + c1 * s2 * s3;
		const double z = c1 * s2 * c3 - s1 * c2 * s3;
		return Quaternion<T>(w, x, y, z);
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

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& lhs, const T& rhs)
	{
		return Quaternion<T>(lhs) *= rhs;;
	}

	template <typename T>
	Quaternion<T> operator*(const T& lhs, const Quaternion<T>& rhs)
	{
		return Quaternion<T>(rhs) *= lhs;
	}
}

#pragma once

#include <ostream>
#include <numbers>

#include "vector.h"
#include "matrix.h"

namespace gml
{
	template<typename T, std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
	class Matrix;

	template<typename T>
	using Mat3 = Matrix<T, 3, 3>;

	template<typename T>
	class Quaternion;

	template<typename T>
	using Quat = Quaternion<T>;

	using Quati = Quaternion<int>;
	using Quatf = Quaternion<float>;
	using Quatd = Quaternion<double>;

	template<typename T>
	constexpr double sqrLength(const Quaternion<T>& q);

	template<typename T>
	constexpr double length(const Quaternion<T>& q);

	template<typename T>
	constexpr Quaternion<T> normalize(Quaternion<T> q);

	template<typename T>
	Quaternion<T> conjugate(Quaternion<T> q);


	/**
	 * @brief Represents a generic quaternion.
	 *
	 * @tparam T The type of the quaternion's components.
	 */
	template<typename T>
	class Quaternion
	{
	public:
		T w, x, y, z; // TODO: make private to be consistent with vectors

		/**
		 * @brief Initializes an identity quaternion.
		 */
		constexpr Quaternion()
			: w(1), x(0), y(0), z(0)
		{
		}

		/**
		 * @brief Initializes the given values.
		 *
		 * @param w The quaternion's w component.
		 * @param x The quaternion's x component.
		 * @param y The quaternion's y component.
		 * @param z The quaternion's z component.
		 */
		constexpr Quaternion(const T& w, const T& x, const T& y, const T& z)
			: w(w), x(x), y(y), z(z)
		{
		}

		/**
		 * @brief Constructs a pure quaternion from a vector.
		 *
		 * @param v The vector to construct from.
		 */
		constexpr explicit Quaternion(const Vec3 <T>& v)
			: w(0), x(v.x()), y(v.y()), z(v.z())
		{
		}

		/**
		 * @brief Normalizes this quaternion.
		 *
		 * @return A reference to this quaternion.
		 */
		constexpr Quaternion<T>& normalize()
		{
			const auto len = length(*this);
			if (len == 0) {
				throw DivideByZeroException();
			}
			return operator*=(1 / len);
		}

		/**
		 * @brief Conjugates this quaternion.
		 *
		 * @return A reference to this quaternion.
		 */
		constexpr Quaternion<T>& conjugate()
		{
			x *= -1;
			y *= -1;
			z *= -1;
			return *this;
		}

		/**
		 * @brief Extracts the Forward vector of the rotation expressed by this quaternion.
		 *
		 * @return the Forward vector
		 */
		[[nodiscard]]
		constexpr Vec3 <T> getForward() const
		{
			Quaternion<T> q = gml::normalize(*this);
			Quaternion<T> result = q * Quaternion<T>(vector::worldForward<T>()) * gml::conjugate(q);
			return gml::normalize(Vec3<T>(result.x, result.y, result.z));
		}

		/**
		 * @brief Extracts the Right vector of the rotation expressed by this quaternion.
		 *
		 * @return the Right vector
		 */
		[[nodiscard]]
		constexpr Vec3 <T> getRight() const
		{
			Quaternion<T> q = gml::normalize(*this);
			Quaternion<T> result = q * Quaternion<T>(vector::worldRight<T>()) * gml::conjugate(q);
			return gml::normalize(Vec3<T>(result.x, result.y, result.z));
		}

		/**
		 * @brief Extracts the Up vector of the rotation expressed by this quaternion.
		 *
		 * @return the Up vector
		 */
		[[nodiscard]]
		constexpr Vec3 <T> getUp() const
		{
			Quaternion<T> q = gml::normalize(*this);
			Quaternion<T> result = q * Quaternion<T>(vector::worldUp<T>()) * gml::conjugate(q);
			return gml::normalize(Vec3<T>(result.x, result.y, result.z));
		}

		/**
		 * @brief Extracts the pitch of the rotation expressed by this quaternion.
		 *
		 * @return the pitch in radians
		 */
		[[nodiscard]]
		constexpr double getPitch() const
		{
			const auto unit = sqrLength(*this);
			const double test = x * y + z * w;
			if (test > 0.499 * unit || test < -0.499 * unit) {
				return 0; // singularity at north and south pole
			}
			const double sqw = w * w;
			const double sqx = x * x;
			const double sqy = y * y;
			const double sqz = z * z;
			return std::atan2(2 * x * w - 2 * y * z, -sqx + sqy - sqz + sqw);
		}


		/**
		* @brief Extracts the yaw of the rotation expressed by this quaternion.
		*
		* @return the yaw in radians
		*/
		[[nodiscard]]
		constexpr double getYaw() const
		{
			const auto unit = sqrLength(*this);
			const double test = x * y + z * w;
			if (test > 0.499 * unit) {
				return 2 * std::atan2(x, w); // singularity at north pole
			}
			if (test < -0.499 * unit) {
				return -2 * std::atan2(x, w); // singularity at south pole
			}
			const double sqw = w * w;
			const double sqx = x * x;
			const double sqy = y * y;
			const double sqz = z * z;
			return std::atan2(2 * y * w - 2 * x * z, sqx - sqy - sqz + sqw);
		}

		/**
		* @brief Extracts the roll of the rotation expressed by this quaternion.
		*
		* @return the roll in radians
		*/
		[[nodiscard]]
		constexpr double getRoll() const
		{
			const auto unit = sqrLength(*this);
			const double test = x * y + z * w;
			if (test > 0.499 * unit) {
				return std::numbers::pi / 2; // singularity at north pole
			}
			if (test < -0.499 * unit) {
				return -std::numbers::pi / 2; // singularity at south pole
			}
			return std::asin(2 * test / unit);
		}

		template<typename T2>
		constexpr Quaternion<T>& operator*=(const T2& rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
			return *this;
		}

		constexpr Quaternion<T>& operator*=(const Quaternion<T>& rhs)
		{
			const Vec3 <T> a = Vec3<T>(x, y, z);
			const Vec3 <T> b = Vec3<T>(rhs.x, rhs.y, rhs.z);

			const Vec3 <T> c = w * b + rhs.w * a + cross(a, b);

			w = w * rhs.w - dot(a, b);
			x = c.x();
			y = c.y();
			z = c.z();
			return *this;
		}

		constexpr Quaternion<T>& operator+=(const Quaternion<T>& rhs)
		{
			w += rhs.w;
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Quaternion<T>& rhs)
	{
		os << "[" << rhs.w << " " << rhs.x << " " << rhs.y << " " << rhs.z << "]";
		return os;
	}

	template<typename T>
	constexpr bool operator==(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}

	template<typename T>
	constexpr bool operator!=(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T>
	constexpr Quaternion<T> operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		return Quaternion<T>(lhs) *= rhs;
	}

    template<typename T>
    constexpr gml::Vec3<T> operator*(const Quaternion<T>& lhs, const Vec3<T>& rhs)
    {
        gml::Vec3<T> u(lhs.x, lhs.y, lhs.z);
        return 2 * gml::dot(u, rhs) * u
               + (lhs.w * lhs.w - gml::dot(u, u)) * rhs
               + 2 * lhs.w * gml::cross(u, rhs);
    }

	template<typename T, typename T2>
	constexpr Quaternion<T> operator*(const Quaternion<T>& lhs, const T2& rhs)
	{
		return Quaternion<T>(lhs) *= rhs;
	}

	template<typename T, typename T2>
	constexpr Quaternion<T> operator*(const T2& lhs, const Quaternion<T>& rhs)
	{
		return Quaternion<T>(rhs) *= lhs;
	}

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
	constexpr Quaternion<T> axisAngle(const Vec3 <T>& axis, const double angle)
	{
		const gml::Vec3<T> a = gml::normalize(axis);
		const double s = std::sin(angle / 2);

		return Quaternion<T>(std::cos(angle / 2), a.x() * s, a.y() * s, a.z() * s);
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
	constexpr Quaternion<T> fromMatrix(Mat3<T> matrix)
	{
		const double tr = trace(matrix);
		Mat3<T>& m = matrix;
		Quaternion<T> q;
		if (tr > 0) {
			const double s = std::sqrt(tr + 1) * 2;
			q.w = 0.25 * s;
			q.x = (m(2, 1) - m(1, 2)) / s;
			q.y = (m(0, 2) - m(2, 0)) / s;
			q.z = (m(1, 0) - m(0, 1)) / s;
		} else if ((m(0, 0) > m(1, 1)) && (m(0, 0) > m(2, 2))) {
			const double s = std::sqrt(1.0 + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
			q.w = (m(2, 1) - m(1, 2)) / s;
			q.x = 0.25 * s;
			q.y = (m(0, 1) + m(1, 0)) / s;
			q.z = (m(0, 2) + m(2, 0)) / s;
		} else if (m(1, 1) > m(2, 2)) {
			const double s = std::sqrt(1.0 + m(1, 1) - m(0, 0) - m(2, 2)) * 2;
			q.w = (m(0, 2) - m(2, 0)) / s;
			q.x = (m(0, 1) + m(1, 0)) / s;
			q.y = 0.25 * s;
			q.z = (m(1, 2) + m(2, 1)) / s;
		} else {
			const double s = std::sqrt(1.0 + m(2, 2) - m(0, 0) - m(1, 1)) * 2;
			q.w = (m(1, 0) - m(0, 1)) / s;
			q.x = (m(0, 2) + m(2, 0)) / s;
			q.y = (m(1, 2) + m(2, 1)) / s;
			q.z = 0.25 * s;
		}
		return q;
	}
}

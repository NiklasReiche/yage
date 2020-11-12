#pragma once

#include <ostream>
#include <numbers>

#include "gml/vector.h"
#include "gml/matrix.h"

namespace gml
{
	template<typename T>
	class Quaternion;

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
		T w, x, y, z;

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
		constexpr explicit Quaternion(const Vec3<T>& v)
			: w(0), x(v.x), y(v.y), z(v.z)
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
		constexpr Vec3<T> getForward() const
		{
			Quaternion<T> q = gml::normalize(*this);
			Quaternion<T> result = q * Quaternion<T>(Vec3<T>::worldForward()) * gml::conjugate(q);
			return gml::normalize(Vec3<T>(result.x, result.y, result.z));
		}

		/**
		 * @brief Extracts the Right vector of the rotation expressed by this quaternion.
		 * 
		 * @return the Right vector
		 */
		[[nodiscard]]
		constexpr Vec3<T> getRight() const
		{
			Quaternion<T> q = gml::normalize(*this);
			Quaternion<T> result = q * Quaternion<T>(Vec3<T>::worldRight()) * gml::conjugate(q);
			return gml::normalize(Vec3<T>(result.x, result.y, result.z));
		}

		/**
		 * @brief Extracts the Up vector of the rotation expressed by this quaternion.
		 * 
		 * @return the Up vector
		 */
		[[nodiscard]]
		constexpr Vec3<T> getUp() const
		{
			Quaternion<T> q = gml::normalize(*this);
			Quaternion<T> result = q * Quaternion<T>(Vec3<T>::worldUp()) * gml::conjugate(q);
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
			Vec3<T> a = Vec3<T>(x, y, z);
			Vec3<T> b = Vec3<T>(rhs.x, rhs.y, rhs.z);

			Vec3<T> c = w * b + rhs.w * a + cross(a, b);

			w = w * rhs.w - dot(a, b);
			x = c.x;
			y = c.y;
			z = c.z;
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
		return lhs.w == rhs.w &&
		       lhs.x == rhs.x &&
		       lhs.y == rhs.y &&
		       lhs.z == rhs.z;
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
}

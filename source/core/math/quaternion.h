#pragma once

#include <ostream>

#include "vector.h"
#include "matrix.h"

namespace gml
{
	template <typename T>
	class Quaternion;

	typedef Quaternion<int> Quati;
	typedef Quaternion<float> Quatf;
	typedef Quaternion<double> Quatd;
	typedef Quaternion<unsigned int> Quatui;

	template <typename T>
	class Mat3;
	template <typename T>
	class Mat4;
	template <typename T>
	class Vec3;

	/**
	 * @brief Represents a generic quaternion.
	 * 
	 * @tparam T the type
	 */
	template <typename T>
	class Quaternion
	{
	public:
		T w, x, y, z;

	public:
		/**
		 * @brief Initializes an identity quaternion.
		 */
		Quaternion();

		/**
		 * @brief Initializes the given values.
		 * 
		 * @param w the w component
		 * @param x the x component
		 * @param y the y component
		 * @param z the z component
		 */
		Quaternion(const T& w, const T& x, const T& y, const T& z);

		/**
		 * @brief Constructs a pure quaternion from a vector
		 * 
		 * @param vector the vector to construct from
		 */
		Quaternion(const Vec3<T> & vector);

	public:
		/**
		* @brief Returns the squared length of this quaternion.
		*
		* @return the squared length
		*/
		double sqrLength() const;

		/**
		 * @brief Returns this quaternion's length.
		 * 
		 * @return the length
		 */
		double length() const;

		/**
		 * @brief Normalizes this quaternion.
		 * 
		 * @return a reference to this quaternion
		 */
		Quaternion<T>& normalize();

		/**
		 * @brief Conjugates this quaternion.
		 * 
		 * @return a reference to this quaternion
		 */
		Quaternion<T>& conjugate();

		/**
		 * @brief Extracts the Forward vector of the rotation expressed by 
		 * this quaternion.
		 * 
		 * @return the Forward vector
		 */
		Vec3<T> getForward() const;

		/**
		 * @brief Extracts the Right vector of the rotation expressed by this
		 * quaternion.
		 * 
		 * @return the Right vector
		 */
		Vec3<T> getRight() const;

		/**
		 * @brief Extracts the Up vector of the rotation expressed by this
		 * quaternion.
		 * 
		 * @return the Up vector
		 */
		Vec3<T> getUp() const;

		/**
		 * @brief Extracts the pitch of the rotation expressed by this
		 * quaternion.
		 * 
		 * @return the pitch in radians
		 */
		double getPitch() const;

		/**
		* @brief Extracts the yaw of the rotation expressed by this
		* quaternion.
		*
		* @return the yaw in radians
		*/
		double getYaw() const;

		/**
		* @brief Extracts the roll of the rotation expressed by this
		* quaternion.
		*
		* @return the roll in radians
		*/
		double getRoll() const;

		/**
		 * @brief Converts this quaternion into a rotation matrix.
		 * 
		 * @return the matrix
		 */
		Mat4<T> toMatrix();

	public:
		/**
		 * @brief Constructs a quaterion from an axis angle representation.
		 * 
		 * @param axis the rotation axis
		 * @param angle the angle in radians
		 * @return the quaternion
		 */
		static Quaternion<T> axisAngle(const Vec3<T>& axis, double angle);

		/**
		 * @brief Constructs a quaterion from an euler angle representation.
		 * 
		 * @param yaw the rotation around the y axis in radians
		 * @param roll the rotation around the z axis in radians
		 * @param pitch the rotation around the x axis in radians
		 * @return the quaternion
		 */
		static Quaternion<T> eulerAngle(double yaw, double roll, double pitch);

		/**
		 * @brief Constructs a quaterion from a rotation matrix.
		 * 
		 * @param matrix the rotation matrix
		 * @return the quaternion
		 */
		static Quaternion<T> rotationMatrix(Mat3<T> matrix);

	public:
		template <typename T2>
		Quaternion<T>& operator*=(const T2& rhs);
		Quaternion<T>& operator*=(const Quaternion<T>& rhs);
	};

	/**
	 * @brief Normalizes the given quaterion.
	 * 
	 * @tparam T the generic type
	 * @param quaternion the quaterion to normalize
	 * @return the normalized quaternion
	 */
	template <typename T>
	Quaternion<T> normalize(const Quaternion<T>& quaternion);

	/**
	 * @brief Returns the conjugate of a given quaternion.
	 * 
	 * @tparam T the generic type
	 * @param quaternion the quaterion to conjugate
	 * @return the conjugate
	 */
	template <typename T>
	Quaternion<T> conjugate(const Quaternion<T>& quaternion);


	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Quaternion<T>& rhs);

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

	template <typename T, typename T2>
	Quaternion<T> operator*(const Quaternion<T>& lhs, const T2& rhs);

	template <typename T, typename T2>
	Quaternion<T> operator*(const T2& lhs, const Quaternion<T>& rhs);
}

#include "quaternion.tpp"

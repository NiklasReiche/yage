#pragma once

#include "matrixBase.h"
#include "mat3.h"
#include "../vector/vec3.h"
#include "../vector/vectorOperations.h"

namespace gml
{
	template <typename T>
	class Quaternion;

	/**
	 * @brief Represents a generic 4x4 matrix.
	 * 
	 * @tparam T The type of the matrix's components.
	 */
	template <typename T>
	class Mat4 : public MatrixBase<T, 4, 4>
	{
	public:
		/**
		 * @brief 4x4 identity matrix.
		 */
		static constexpr const Mat4<T> Id = gml::Id<T, 4>;

		using MatrixBase<T, 4, 4>::MatrixBase;

		/**
		 * @brief Implicit conversion from MatrixBase<T, 4, 4>.
		 */
		constexpr Mat4(const MatrixBase<T, 4, 4>& other)
			: MatrixBase<T, 4, 4>(other)
		{
		}

		/**
		* @brief Returns the translation component of this matrix.
		*
		* @return the translation vector
		*/
		constexpr Vec3<T> getTranslation() const
		{
			return Vec3<T>((*this)(0, 3), (*this)(1, 3), (*this)(2, 3));
		}

		/**
		* @brief Returns the rotation component of this matrix as a matrix.
		*
		* @return the rotation matrix
		*/
		constexpr Mat3<T> getRotation() const
		{
			Vec3<T> scale = getScale();
			return Mat3<T>{
				(*this)(0, 0) / scale.x, (*this)(0, 1) / scale.y, (*this)(0, 2) / scale.z,
				(*this)(1, 0) / scale.x, (*this)(1, 1) / scale.y, (*this)(1, 2) / scale.z,
				(*this)(2, 0) / scale.x, (*this)(2, 1) / scale.y, (*this)(2, 2) / scale.z
			};
		}

		/**
		* @brief Returns the scaling component of this matrix.
		*
		* @return the scaling vector
		*/
		constexpr Vec3<T> getScale() const
		{
			return Vec3<T>{
				length(Vec3<T>((*this)(0, 0), (*this)(1, 0), (*this)(2, 0))),
				length(Vec3<T>((*this)(0, 1), (*this)(1, 1), (*this)(2, 1))),
				length(Vec3<T>((*this)(0, 2), (*this)(1, 2), (*this)(2, 2)))
			};
		}

		/**
		* @brief Converts this matrix to a quaternion.
		 *
		* This assumes that the matrix is a pure rotation matrix.
		*
		* @return the quaternion
		*/
		Quaternion<T> toQuaternion()
		{
			return Quaternion<T>::rotationMatrix(*this);
		}

		/**
		* @brief Constructs a translation matrix.
		*
		* @param x the x component
		* @param y the y component
		* @param z the z component
		* @return the translation matrix
		*/
		static Mat4<T> translate(const T& x, const T& y, const T& z);

		/**
		* @brief Constructs a translation matrix from a vector.
		*
		* @param translation the translation vector
		* @return the translation matrix
		*/
		static Mat4<T> translate(const Vec3<T>& translation);

		/**
		* @brief Constructs a scaling matrix.
		*
		* @param x the x component
		* @param y the y component
		* @param z the z component
		* @return the scaling matrix
		*/
		static Mat4<T> scale(const T& x, const T& y, const T& z);

		/**
		* @brief Constructs a scaling matrix from a vector.
		*
		* @param scale the scaling vector
		* @return the scaling matrix
		*/
		static Mat4<T> scale(const Vec3<T>& scale);

		/**
		* @brief Constructs a rotation matrix from an axis angle 
		* representation.
		*
		* @param axis the rotation axis
		* @param angle the angle in radians
		* @return the rotation matrix
		*/
		static Mat4<T> axisAngle(const Vec3<T>& axis, double angle);

		/**
		* @brief Constructs a rotation matrix from a quaternion 
		* representation.
		*
		* @return the rotation matrix
		*/
		static Mat4<T> quaternion(const Quaternion<T>& quaternion);

		/**
		* @brief Constructs a perspective projection matrix.
		*
		* @param fov the field of view in degrees
		* @param aspect the aspect ration
		* @param near the near plane
		* @param far the far plane
		* @return the projection matrix
		*/
		static Mat4<T> perspective(double fov, double aspect, double near, double far);

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
		static Mat4<T> orthographic(double left, double right, double bottom, double top, double near, double far);

		/**
		* @brief Constructs a lookAt view matrix.
		*
		* @param pos the camera position
		* @param target the view target position
		* @param up the camera up vector
		* @return the view matrix
		*/
		static Mat4<T> lookAt(const Vec3<T>& pos, const Vec3<T>& target, const Vec3<T>& up);
	};
}

#include "mat4.tpp"

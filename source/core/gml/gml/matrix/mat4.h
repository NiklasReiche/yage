#pragma once

#include "matrixBase.h"
#include "mat3.h"
#include "../vector/vec3.h"

namespace gml
{
	template <typename T>
	class Quaternion;

	/**
	 * @brief Represents a generic 4x4 matrix.
	 * 
	 * @tparam T the generic type
	 */
	template <typename T>
	class Mat4 : public MatrixBase<T, 4, 4>
	{
	public:
		/** Inherit Constructors from MatrixBase */
		using MatrixBase<T, 4, 4>::MatrixBase;

		/**
		 * @brief Constructs an identity matrix.
		 */
		constexpr Mat4()
			: MatrixBase<T, 4, 4>()
		{
		}

		constexpr Mat4(const Mat4<T>& other)
			: MatrixBase<T, 4, 4>(other)
		{
		}

		constexpr Mat4(const MatrixBase<T, 4, 4>& other)
			: MatrixBase<T, 4, 4>(other)
		{
		}

		static constexpr const Mat4<T> Identity = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		/**
		* @brief Returns the translation component of this matrix.
		*
		* @return the translation vector
		*/
		Vec3<T> getTranslation() const;

		/**
		* @brief Returns the rotation component of this matrix as a matrix.
		*
		* @return the rotation matrix
		*/
		Mat3<T> getRotation() const;

		/**
		* @brief Returns the scaling component of this matrix.
		*
		* @return the scaling vector
		*/
		Vec3<T> getScale() const;

		/**
		* @brief Converts this matrix to a quaternion.
		* This assumes that the matrix is a pure rotation matrix
		*
		* @return the quaternion
		*/
		Quaternion<T> toQuaternion();

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

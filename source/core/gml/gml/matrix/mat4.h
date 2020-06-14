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
		* @return The translation vector.
		*/
		constexpr Vec3<T> getTranslation() const
		{
			return Vec3<T>((*this)(0, 3), (*this)(1, 3), (*this)(2, 3));
		}

		/**
		* @brief Returns the rotation component of this matrix as a matrix.
		*
		* @return The rotation matrix.
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
		* @return The scaling vector.
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
		* @return The quaternion.
		*/
		Quaternion<T> toQuaternion()
		{
			return Quaternion<T>::rotationMatrix(*this);
		}
	};
}
#pragma once

#include "matrixBase.h"

namespace gml
{
	template <typename T>
	class Quaternion;

	/**
	* @brief Represents a generic 3x3 matrix.
	*
	* @tparam T the generic type
	*/
	template <typename T>
	class Mat3 : public MatrixBase<T, 3, 3>
	{
	public:
		/** Inherit Constructors from MatrixBase */
		using MatrixBase<T, 3, 3>::MatrixBase;

		/**
		* @brief Constructs an identity matrix.
		*/
		constexpr Mat3()
			: MatrixBase<T, 3, 3>()
		{
		}

		constexpr Mat3(const Mat3<T>& other)
			: MatrixBase<T, 3, 3>(other)
		{
		}

		constexpr Mat3(const MatrixBase<T, 3, 3>& other)
			: MatrixBase<T, 3, 3>(other)
		{
		}

		static constexpr const Mat3<T> Identity = {
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 }
		};

		/**
		 * @brief Converts this matrix to a quaternion. 
		 * This assumes that the matrix is a pure rotation matrix
		 * 
		 * @return the quaternion
		 */
		Quaternion<T> toQuaternion()
		{
			return Quaternion<T>::rotationMatrix(*this);
		}
	};
}

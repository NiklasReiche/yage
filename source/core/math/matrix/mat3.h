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
		Mat3();

		Mat3(const Mat3<T>& other);

		Mat3(const MatrixBase<T, 3, 3>& other);

		/**
		 * @brief Converts this matrix to a quaternion. 
		 * This assumes that the matrix is a pure rotation matrix
		 * 
		 * @return the quaternion
		 */
		Quaternion<T> toQuaternion();
	};
}

#include "mat3.tpp"

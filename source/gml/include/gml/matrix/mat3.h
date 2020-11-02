#pragma once

#include "matrixBase.h"

namespace gml
{
	template <typename T>
	class Quaternion;

	/**
	* @brief Represents a generic 3x3 matrix.
	*
	* @tparam T The type of the matrix's components.
	*/
	template <typename T>
	class Mat3 : public MatrixBase<T, 3, 3>
	{
	public:
		/**
		 * @brief 3x3 identity matrix.
		 */
		static constexpr const Mat3<T> Id = gml::Id<T, 3>;

		using MatrixBase<T, 3, 3>::MatrixBase;

		/**
		 * @brief Implicit conversion from MatrixBase<T, 3, 3>.
		 */
		constexpr Mat3(const MatrixBase<T, 3, 3>& other)
			: MatrixBase<T, 3, 3>(other)
		{
		}

		/**
		 * @brief Converts this matrix to a quaternion.
		 *
		 * Assumes that this matrix is a pure rotation matrix.
		 * 
		 * @return A quaternion representing the same rotation as this matrix.
		 */
		constexpr Quaternion<T> toQuaternion()
		{
			return Quaternion<T>::rotationMatrix(*this);
		}
	};
}

#pragma once

#include "matrixBase.h"

namespace gml
{
	/**
	* @brief Represents a generic 2x2 matrix.
	*
	* @tparam T The type of the matrix's components.
	*/
	template <typename T>
	class Mat2 : public MatrixBase<T, 2, 2>
	{
	public:
		/**
		 * @brief 2x2 identity matrix.
		 */
		static constexpr const Mat2<T> Id = gml::Id<T, 2>;

		using MatrixBase<T, 2, 2>::MatrixBase;

		/**
		 * @brief Implicit conversion from MatrixBase<T, 2, 2>.
		 */
		constexpr Mat2(const MatrixBase<T, 2, 2>& other)
			: MatrixBase<T, 2, 2>(other)
		{
		}
	};
}

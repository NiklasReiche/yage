#pragma once

#include "matrixBase.h"

namespace gml
{
	/**
	* @brief Represents a generic 2x2 matrix.
	*
	* @tparam T the generic type
	*/
	template <typename T>
	class Mat2 : public MatrixBase<T, 2, 2>
	{
	public:
		/** Inherit Constructors from MatrixBase */
		using MatrixBase<T, 2, 2>::MatrixBase;

		/**
		* @brief Constructs an identity matrix.
		*/
		constexpr Mat2()
			: MatrixBase<T, 2, 2>()
		{
		}

		constexpr Mat2(const Mat2<T>& other)
			: MatrixBase<T, 2, 2>(other)
		{
		}

		constexpr Mat2(const MatrixBase<T, 2, 2>& other)
			: MatrixBase<T, 2, 2>(other)
		{
		}

		static constexpr const Mat2<T> Identity = {
			{ 1, 0 },
			{ 0, 1 }
		};
	};
}

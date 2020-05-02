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
		Mat2();

		Mat2(const Mat2<T>& other);

		Mat2(const MatrixBase<T, 2, 2>& other);
	};
}

#include "mat2.tpp"

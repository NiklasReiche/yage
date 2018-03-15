#pragma once

namespace gml
{
	template <typename T>
	Mat2<T>::Mat2()
		: MatrixBase()
	{
	}

	template <typename T>
	Mat2<T>::Mat2(const Mat2<T>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat2<T>::Mat2(const MatrixBase<T, 2, 2>& other)
		: MatrixBase(other)
	{
	}
}

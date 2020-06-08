#pragma once

namespace gml
{
	template <typename T, std::size_t Rows, std::size_t Columns>
	class MatrixBase;

	/* 0x0 matrices are not allowed */
	template<typename T>
	class MatrixBase<T, 0, 0>
	{
	};

	template<typename T, std::size_t M>
	class MatrixBase<T, M, 0>
	{
	};

	template<typename T, std::size_t N>
	class MatrixBase<T, 0, N>
	{
	};
}
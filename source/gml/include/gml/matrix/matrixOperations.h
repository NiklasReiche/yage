#pragma once

#include "matrixBase.h"

namespace gml
{
	/**
	 * @brief Returns a matrix with a given row and column removed.
	 *
	 * @param row The index of the row to remove.
	 * @param column The index of the column to remove.
	 * @return The reduced matrix.
	 *
	 * @throws InvalidDimensionException if row or column is out of bounds
	 */
	template<typename T, std::size_t M, std::size_t N>
	[[nodiscard]]
	constexpr MatrixBase<T, M - 1, N - 1>
	reduce(const MatrixBase <T, M, N>& matrix, std::size_t row, std::size_t column)
	{
		if (row > M || column > N) {
			throw InvalidDimensionException();
		}

		MatrixBase < T, M - 1, N - 1 > result;

		for (std::size_t i = 0; i < M - 1; ++i) {
			const auto rowI = (i < row) ? i : i + 1;
			for (std::size_t j = 0; j < N - 1; ++j) {
				const auto colI = (j < column) ? j : j + 1;
				result(i, j) = matrix(rowI, colI);
			}
		}
		return result;
	}

	/**
	 * @brief Calculates the determinant of a quadratic matrix.
	 *
	 * @tparam T The type of the matrix's components.
	 * @tparam N The number of columns or rows of the matrix.
	 * @param matrix The matrix for which to calculate the determinant.
	 * @return The matrix's determinant.
	 */
	template<typename T, std::size_t N>
	[[nodiscard]]
	constexpr double det(const MatrixBase <T, N, N>& matrix)
	{
		double result = 0;
		for (std::size_t i = 0; i < N; ++i) {
			result += std::pow(-1, i + 1) * matrix(1, i) * det(reduce(matrix, 1, i));
		}
		return result;
	}

	template<typename T>
	[[nodiscard]]
	constexpr double det(const MatrixBase<T, 1, 1>& matrix)
	{
		return matrix(0, 0);
	}

	template<typename T>
	[[nodiscard]]
	constexpr double det(const MatrixBase<T, 2, 2>& matrix)
	{
		return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
	}

	template<typename T>
	[[nodiscard]]
	constexpr double det(const MatrixBase<T, 3, 3>& matrix)
	{
		return matrix(0, 0) * matrix(1, 1) * matrix(2, 2) +
		       matrix(0, 1) * matrix(1, 2) * matrix(2, 0) +
		       matrix(0, 2) * matrix(1, 0) * matrix(2, 1) -
		       matrix(0, 2) * matrix(1, 1) * matrix(2, 0) -
		       matrix(0, 1) * matrix(1, 0) * matrix(2, 2) -
		       matrix(0, 0) * matrix(1, 2) * matrix(2, 1);
	}

	/**
	 * @brief Calculates the trace of a square matrix.
	 *
	 * @tparam T The type of the matrix's components.
	 * @tparam N The number of rows or columns of the matrix.
	 * @param matrix The matrix for which to calculate the trace.
	 * @return The matrix's trace.
	 */
	template<typename T, std::size_t N>
	[[nodiscard]]
	constexpr double trace(const MatrixBase <T, N, N>& matrix)
	{
		double trace = 0;
		for (std::size_t i = 0; i < N; ++i) {
			trace += matrix(i, i);
		}
		return trace;
	}

	/**
	 * @brief Calculates the transpose of a matrix.
	 *
	 * @tparam T The type of the matrix's components.
	 * @tparam M The number of rows of the matrix.
	 * @tparam N The number of columns of the matrix.
	 * @param matrix The matrix for which to calculate the transpose.
	 * @return The matrix's transpose matrix.
	 */
	template<typename T, std::size_t M, std::size_t N>
	[[nodiscard]]
	constexpr MatrixBase <T, N, M> transpose(const MatrixBase <T, M, N>& matrix)
	{
		MatrixBase <T, N, M> result;
		for (std::size_t i = 0; i < M; ++i) {
			for (std::size_t j = 0; j < N; ++j) {
				result(j, i) = matrix(i, j);
			}
		}
		return result;
	}

	/**
	 * @brief Calculates the inverse of a square matrix.
	 *
	 * Uses the Gauss-Jordan Elimination algorithm. If the matrix is not invertible an exception is thrown.
	 *
	 * @tparam T The type of the matrix's components.
	 * @tparam N The number of rows or columns of the matrix.
	 * @param matrix The matrix for which to calculate the inverse.
	 * @return The matrix's inverse matrix.
	 *
	 * @throws DivideByZeroException The determinant of the matrix is zero.
	 */
	template<typename T, std::size_t N>
	[[nodiscard]]
	constexpr MatrixBase <T, N, N> inverse(const MatrixBase <T, N, N>& matrix)
	{
		if (det(matrix) == 0) {
			throw DivideByZeroException();
		}

		MatrixBase <T, N, N> mat = matrix;
		MatrixBase <T, N, N> inv = gml::Id<T, N>;

		auto pivotSearch = [&mat](std::size_t i_min)
		{
			std::size_t i_max = i_min;
			for (std::size_t i = i_min + 1; i < N; ++i) {
				if (std::abs(mat(i, i_min)) > std::abs(mat(i_max, i_min))) {
					i_max = i;
				}
			}
			return i_max;
		};

		// transform to lower triangular form
		for (std::size_t i = 0; i < N; ++i) {
			// column pivot search for numeric stability
			auto i_pivot = pivotSearch(i);
			if (mat(i_pivot, i) == 0) {
				throw DivideByZeroException();
			}
			if (i_pivot != i) {
				mat.switchRows(i_pivot, i);
				inv.switchRows(i_pivot, i);
			}

			// normalize the pivot
			T value = mat(i, i);
			mat.combineRows(i, 1 / value, 0, 0);
			inv.combineRows(i, 1 / value, 0, 0);

			// reduce remaining column to zero
			for (std::size_t j = i + 1; j < N; ++j) {
				T factor = -mat(j, i);
				mat.combineRows(j, 1, i, factor);
				inv.combineRows(j, 1, i, factor);
			}
		}

		// reduce values above diagonal to zero
		for (std::size_t i = N; i-- > 1;) // prevent unsigned overflow
		{
			for (std::size_t j = i; j-- > 0;) // prevent unsigned overflow
			{
				T factor = -mat(j, i);
				mat.combineRows(j, 1, i, factor);
				inv.combineRows(j, 1, i, factor);
			}
		}

		return inv;
	}

	template<typename T>
	[[nodiscard]]
	constexpr MatrixBase<T, 2, 2> inverse(const MatrixBase<T, 2, 2>& matrix)
	{
		const double m_det = det(matrix);
		if (m_det == 0) {
			throw DivideByZeroException();
		}

		MatrixBase<T, 2, 2> inv{
			matrix(1, 1), -matrix(0, 1),
			-matrix(1, 0), matrix(0, 0)
		};
		return inv / m_det;
	}

	template<typename T>
	[[nodiscard]]
	constexpr MatrixBase<T, 3, 3> inverse(const MatrixBase<T, 3, 3>& matrix)
	{
		const double m_det = det(matrix);
		if (m_det == 0) {
			throw DivideByZeroException();
		}

		auto& m = matrix;
		MatrixBase<T, 3, 3> inv{
			m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1), m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2),
			m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1),
			m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2), m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0),
			m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2),
			m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0), m(0, 1) * m(2, 0) - m(0, 0) * m(2, 1),
			m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)
		};
		return inv / m_det;
	}
}

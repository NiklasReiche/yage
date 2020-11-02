#pragma once

#include <array>
#include <initializer_list>
#include <ostream>
#include <span>

#include "../constraints.h"
#include "../exception.h"
#include "../maths.h"
#include "../vector/vectorBase.h"

namespace gml
{
	template<typename T, std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
	class MatrixBase;

	/**
	 * @brief The NxN identity matrix.
	 *
	 * @tparam T The element type of the matrix.
	 * @tparam N The dimension of the identity matrix.
	 */
	template<typename T, std::size_t N>
	constexpr const MatrixBase<T, N, N> Id = []
	{
		MatrixBase<T, N, N> m;
		for (std::size_t i = 0; i < N; ++i) {
			m(i, i) = 1;
		}
		return m;
	}();

	/**
	 * @brief Represents a generic NxM dimensional matrix.
	 *
	 * The dimensions must be greater than zero.
	 * 
	 * @tparam T The type of the matrix's components.
	 * @tparam M The number of rows.
	 * @tparam N The number of columns.
	 */
	template<typename T, std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
	class MatrixBase
	{
	public:
		/**
		 * @brief Value-initializes all components.
		 */
		constexpr MatrixBase()
			: elements{ }
		{
		}

		/**
		 * @brief Initializes all components with the same value.
		 *
		 * @param value The default value used for initialization.
		 */
		constexpr explicit MatrixBase(const T& value)
		{
			elements.fill(value);
		}

		/**
		 * @brief Initializes the components from a continuous memory block.
		 *
		 * The data is read in row-major format.
		 *
		 * @param data Continuous memory to copy elements from.
		 */
		constexpr explicit MatrixBase(std::span<T, N * M> data)
		{
			std::copy(data.begin(), data.end(), elements.begin());
		}

		/**
		 * @brief Initializes the components from a one-dimensional initializer list.
		 * 
		 * @param init The initializer list.
		 */
		constexpr MatrixBase(const std::initializer_list<T>& init)
		{
			if (init.size() != M * N) {
				throw InvalidDimensionException();
			}
			std::copy(std::begin(init), std::end(init), std::begin(elements));
		}

		/**
		 * @brief Initializes the components from a two-dimensional initializer list.
		 *
		 * @param init The initializer list.
		 */
		constexpr MatrixBase(const std::initializer_list<std::initializer_list<T>>& init)
		{
			if (init.size() != M) {
				throw InvalidDimensionException();
			}

			std::size_t i = 0;
			for (const auto& row : init) {
				if (row.size() != N) {
					throw InvalidDimensionException();
				}
				std::copy(std::begin(row), std::end(row), std::begin(elements) + i * N);
				++i;
			}
		}

		/**
		 * @brief Constructs a matrix by copy-converting the components of another matrix.
		 *
		 * Type T of the constructed matrix must to be assignable from type T2 of the matrix being copied.
		 *
		 * @tparam T2 Type of the components of the vector from which to convert.
		 * @param other Matrix from which to copy and convert the components.
		 */
		template<typename T2>
		constexpr MatrixBase(const MatrixBase<T2, M, N>& other)
		{
			std::copy(other.data(), other.data() + (N * M), elements.begin());
		}

		/**
		 * @brief Assigns this vector by copy-converting the components of another vector.
		 *
		 * Type T of the constructed matrix must to be assignable from type T2 of the copied matrix.
		 *
		 * @tparam T2 Type of the components of the vector from which to convert.
		 * @param other Matrix from which to copy and convert the components.
		 * @return This matrix.
		 */
		template<typename T2>
		constexpr MatrixBase<T, M, N>& operator=(const MatrixBase<T2, M, N>& other)
		{
			std::copy(other.data(), other.data() + (N * M), elements.begin());
			return *this;
		}

		/**
		 * @brief Gives direct access to the underlying array.
		 *
		 * @return Pointer to the underlying element storage (equal to the address of the first element).
		 */
		constexpr T* data() noexcept
		{
			return elements.data();
		}

		/**
		 * @brief Gives direct access to the underlying array.
		 *
		 * @return Pointer to the underlying element storage (equal to the address of the first element).
		 */
		constexpr const T* data() const noexcept
		{
			return elements.data();
		}

		/**
		 * @brief Returns the reference to an element of this matrix.
		 *
		 * @param m The row index of the element.
		 * @param n The column index of the element.
		 * @return The reference to the requested element.
		 *
		 * @throws std::out_of_range If !(m < M && n < N).
		 */
		constexpr T& operator()(int m, int n)
		{
			return elements.at(m * N + n);
		}

		/**
		 * @brief Returns the reference to an element of this matrix.
		 *
		 * @param m The row index of the element.
		 * @param n The column index of the element.
		 * @return The reference to the requested element.
		 *
		 * @throws std::out_of_range If !(m < M && n < N).
		 */
		constexpr const T& operator()(int m, int n) const
		{
			return elements.at(m * N + n);
		}

		/**
		 * @brief Element-wise addition.
		 */
		constexpr MatrixBase<T, M, N>& operator+=(const MatrixBase<T, M, N>& rhs)
		{
			for (std::size_t i = 0; i < M * N; ++i) {
				elements.at(i) += rhs.elements.at(i);
			}
			return *this;
		}

		/**
		 * @brief Element-wise subtraction.
		 */
		constexpr MatrixBase<T, M, N>& operator-=(const MatrixBase<T, M, N>& rhs)
		{
			for (std::size_t i = 0; i < M * N; ++i) {
				elements.at(i) -= rhs.elements.at(i);
			}
			return *this;
		}

		/**
		 * @brief Element-wise scalar multiplication.
		 */
		constexpr MatrixBase<T, M, N>& operator*=(const T& rhs)
		{
			for (std::size_t i = 0; i < M * N; ++i) {
				elements.at(i) *= rhs;
			}
			return *this;
		}

		/**
		 * @brief Matrix multiplication with a square matrix.
		 */
		constexpr MatrixBase<T, M, N>& operator*=(const MatrixBase<T, N, N>& rhs)
		{
			return operator=(*this * rhs);
		}

		/**
		 * @brief Element-wise scalar division.
		 */
		constexpr MatrixBase<T, M, N>& operator/=(const T& rhs)
		{
			if (rhs == 0) {
				throw DivideByZeroException();
			}
			for (std::size_t i = 0; i < M * N; ++i) {
				elements.at(i) /= rhs;
			}
			return *this;
		}

		/**
		* @brief Switch two rows in this matrix.
		*
		* @param first The index of the first row to switch with the other row.
		* @param second The index of the second row to switch with the other row.
		*/
		constexpr void switchRows(std::size_t first, std::size_t second)
		{
			for (std::size_t i = 0; i < N; i++) {
				T temp = operator()(first, i);
				operator()(first, i) = operator()(second, i);
				operator()(second, i) = temp;
			}
		}

		/**
		 * @brief Adds a multiplied row to another multiplied row in this matrix.
		 *
		 * The row is modified by the following equation: rowA = a * rowA + b * rowB
		 *
		 * @param rowA The index of the row to modify.
		 * @param a The factor by which to multiply rowA.
		 * @param rowB The index of the row to add to the modified rowA.
		 * @param b The factor by which to multiply rowB before adding it to rowA.
		 */
		constexpr void combineRows(std::size_t rowA, double a, std::size_t rowB, double b)
		{
			for (std::size_t i = 0; i < N; ++i) {
				operator()(rowA, i) = operator()(rowA, i) * a + operator()(rowB, i) * b;
			}
		}

		template<typename T_, std::size_t M_, std::size_t N_>
		friend constexpr bool operator==(const MatrixBase<T_, M_, N_>& left, const MatrixBase<T_, M_, N_>& right);

	private:
		/**
		 * @brief The matrix components.
		 */
		std::array<T, N * M> elements;
	};

	template<typename T, std::size_t M, std::size_t N>
	std::ostream& operator<<(std::ostream& os, const MatrixBase<T, M, N>& matrix)
	{
		os << "\n[ ";
		for (std::size_t i = 0; i < M; ++i) {
			for (std::size_t j = 0; j < N; ++j) {
				os << (i == 0 && j == 0 ? "" : "  ") << (matrix(i, j) < 0 ? "" : " ") << matrix(i, j);
			}
			os << (i < M - 1 ? "\n" : " ]\n");
		}
		return os;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr bool operator==(const MatrixBase<T, M, N>& left, const MatrixBase<T, M, N>& right)
	{
		return left.elements == right.elements;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr bool operator!=(const MatrixBase<T, M, N>& left, const MatrixBase<T, M, N>& right)
	{
		return !(left == right);
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr MatrixBase<T, M, N> operator+(const MatrixBase<T, M, N>& left, const MatrixBase<T, M, N>& right)
	{
		return MatrixBase<T, M, N>(left) += right;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr MatrixBase<T, M, N> operator-(const MatrixBase<T, M, N>& left, const MatrixBase<T, M, N>& right)
	{
		return MatrixBase<T, M, N>(left) -= right;
	}

	template<typename T, std::size_t M, std::size_t N, std::size_t P>
	constexpr MatrixBase<T, M, P> operator*(const MatrixBase<T, M, N>& left, const MatrixBase<T, N, P>& right)
	{
		MatrixBase<T, M, P> result;
		for (std::size_t i = 0; i < M; ++i) {
			for (std::size_t j = 0; j < P; ++j) {
				T value = 0;
				for (std::size_t k = 0; k < N; ++k) {
					value += left(i, k) * right(k, j);
				}
				result(i, j) = value;
			}
		}
		return result;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr MatrixBase<T, M, N> operator*(const T& left, const MatrixBase<T, M, N>& right)
	{
		return MatrixBase<T, M, N>(right) *= left;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr MatrixBase<T, M, N> operator*(const MatrixBase<T, M, N>& left, const T& right)
	{
		return MatrixBase<T, M, N>(left) *= right;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr VectorBase<T, M> operator*(const MatrixBase<T, M, N>& left, const VectorBase<T, N>& right)
	{
		VectorBase<T, M> result;
		for (std::size_t i = 0; i < M; ++i) {
			for (std::size_t j = 0; j < N; ++j) {
				result(i) += left(i, j) * right(j);
			}
		}
		return result;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr MatrixBase<T, M, N> operator/(const MatrixBase<T, M, N>& left, const T& right)
	{
		return MatrixBase<T, M, N>(left) /= right;
	}
}

#pragma once

#include <array>
#include <initializer_list>
#include <ostream>
#include <span>

#include "constraints.h"
#include "exception.h"
#include "maths.h"
#include "vector.h"
#include "quaternion.h"

namespace gml
{
	template<typename T>
	class Quaternion;

	template<typename T, std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
	class Matrix;

	template<typename T>
	using Mat2 = Matrix<T, 2, 2>;

	using Mat2i = Mat2<int>;
	using Mat2f = Mat2<float>;
	using Mat2d = Mat2<double>;

	template<typename T>
	using Mat3 = Matrix<T, 3, 3>;

	using Mat3i = Mat3<int>;
	using Mat3f = Mat3<float>;
	using Mat3d = Mat3<double>;

	template<typename T>
	using Mat4 = Matrix<T, 4, 4>;

	using Mat4i = Mat4<int>;
	using Mat4f = Mat4<float>;
	using Mat4d = Mat4<double>;

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
	class Matrix
	{
	public:
		/**
		 * @brief Value-initializes all components.
		 */
		constexpr Matrix()
			: elements{ }
		{
		}

		/**
		 * @brief Initializes all components with the same value.
		 *
		 * @param value The default value used for initialization.
		 */
		constexpr explicit Matrix(const T& value)
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
		constexpr explicit Matrix(std::span<T, N * M> data)
		{
			std::copy(data.begin(), data.end(), elements.begin());
		}

		/**
		 * @brief Initializes the components from a one-dimensional initializer list.
		 * 
		 * @param init The initializer list.
		 */
		constexpr Matrix(const std::initializer_list<T>& init)
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
		constexpr Matrix(const std::initializer_list<std::initializer_list<T>>& init)
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
		constexpr Matrix(const Matrix<T2, M, N>& other)
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
		constexpr Matrix<T, M, N>& operator=(const Matrix<T2, M, N>& other)
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
		constexpr Matrix<T, M, N>& operator+=(const Matrix<T, M, N>& rhs)
		{
			for (std::size_t i = 0; i < M * N; ++i) {
				elements.at(i) += rhs.elements.at(i);
			}
			return *this;
		}

		/**
		 * @brief Element-wise subtraction.
		 */
		constexpr Matrix<T, M, N>& operator-=(const Matrix<T, M, N>& rhs)
		{
			for (std::size_t i = 0; i < M * N; ++i) {
				elements.at(i) -= rhs.elements.at(i);
			}
			return *this;
		}

		/**
		 * @brief Element-wise scalar multiplication.
		 */
		constexpr Matrix<T, M, N>& operator*=(const T& rhs)
		{
			for (std::size_t i = 0; i < M * N; ++i) {
				elements.at(i) *= rhs;
			}
			return *this;
		}

		/**
		 * @brief Matrix multiplication with a square matrix.
		 */
		constexpr Matrix<T, M, N>& operator*=(const Matrix<T, N, N>& rhs)
		{
			return operator=(*this * rhs);
		}

		/**
		 * @brief Element-wise scalar division.
		 */
		constexpr Matrix<T, M, N>& operator/=(const T& rhs)
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

		/**
		* @brief Returns the translation component of this matrix.
		*
		* @return The translation vector.
		*/
		constexpr Vector<T, 3> getTranslation() const requires (N == 4 && M == 4)
		{
			return Vector<T, 3>{ (*this)(0, 3), (*this)(1, 3), (*this)(2, 3) };
		}

		/**
		* @brief Returns the rotation component of this matrix as a matrix.
		*
		* @return The rotation matrix.
		*/
		constexpr Matrix<T, 3, 3> getRotation() const requires (N == 4 && M == 4)
		{
			const Vector<T, 3> scale = getScale();
			return Matrix<T, 3, 3>{
				(*this)(0, 0) / scale.x(), (*this)(0, 1) / scale.y(), (*this)(0, 2) / scale.z(),
				(*this)(1, 0) / scale.x(), (*this)(1, 1) / scale.y(), (*this)(1, 2) / scale.z(),
				(*this)(2, 0) / scale.x(), (*this)(2, 1) / scale.y(), (*this)(2, 2) / scale.z()
			};
		}

		/**
		* @brief Returns the scaling component of this matrix.
		*
		* @return The scaling vector.
		*/
		constexpr Vector<T, 3> getScale() const requires (N == 4 && M == 4)
		{
			return Vector<T, 3>{
				length(Vector<T, 3>{ (*this)(0, 0), (*this)(1, 0), (*this)(2, 0) }),
				length(Vector<T, 3>{ (*this)(0, 1), (*this)(1, 1), (*this)(2, 1) }),
				length(Vector<T, 3>{ (*this)(0, 2), (*this)(1, 2), (*this)(2, 2) })
			};
		}


		template<typename T_, std::size_t M_, std::size_t N_>
		friend constexpr bool operator==(const Matrix<T_, M_, N_>& left, const Matrix<T_, M_, N_>& right);

	private:
		/**
		 * @brief The matrix components.
		 */
		std::array<T, N * M> elements;
	};

	template<typename T, std::size_t M, std::size_t N>
	std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& matrix)
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
	constexpr bool operator==(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
	{
		return left.elements == right.elements;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr bool operator!=(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
	{
		return !(left == right);
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr Matrix<T, M, N> operator+(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
	{
		return Matrix<T, M, N>(left) += right;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr Matrix<T, M, N> operator-(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
	{
		return Matrix<T, M, N>(left) -= right;
	}

	template<typename T, std::size_t M, std::size_t N, std::size_t P>
	constexpr Matrix<T, M, P> operator*(const Matrix<T, M, N>& left, const Matrix<T, N, P>& right)
	{
		Matrix<T, M, P> result;
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
	constexpr Matrix<T, M, N> operator*(const T& left, const Matrix<T, M, N>& right)
	{
		return Matrix<T, M, N>(right) *= left;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr Matrix<T, M, N> operator*(const Matrix<T, M, N>& left, const T& right)
	{
		return Matrix<T, M, N>(left) *= right;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr Vector<T, M> operator*(const Matrix<T, M, N>& left, const Vector<T, N>& right)
	{
		Vector<T, M> result;
		for (std::size_t i = 0; i < M; ++i) {
			for (std::size_t j = 0; j < N; ++j) {
				result(i) += left(i, j) * right(j);
			}
		}
		return result;
	}

	template<typename T, std::size_t M, std::size_t N>
	constexpr Matrix<T, M, N> operator/(const Matrix<T, M, N>& left, const T& right)
	{
		return Matrix<T, M, N>(left) /= right;
	}

	namespace matrix
	{
		/**
	 	 * @brief The NxN identity matrix.
	 	 *
	 	 * @tparam T The element type of the matrix.
	 	 * @tparam N The dimension of the identity matrix.
	 	 */
		template<typename T, std::size_t N>
		inline constexpr auto Id = []() constexpr
		{
			Matrix<T, N, N> m;
			for (std::size_t i = 0; i < N; ++i) {
				m(i, i) = 1;
			}
			return m;
		}();

		template<typename T>
		inline constexpr auto& Id1 = Id<T, 1>;

		template<typename T>
		inline constexpr auto& Id2 = Id<T, 2>;
		inline constexpr auto& Id2f = Id<float, 2>;
		inline constexpr auto& Id2d = Id<double, 2>;

		template<typename T>
		inline constexpr auto& Id3 = Id<T, 3>;
		inline constexpr auto& Id3f = Id<float, 3>;
		inline constexpr auto& Id3d = Id<double, 3>;

		template<typename T>
		inline constexpr auto& Id4 = Id<T, 4>;
		inline constexpr auto& Id4f = Id<float, 4>;
		inline constexpr auto& Id4d = Id<double, 4>;
	}

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
	constexpr Matrix<T, M - 1, N - 1>
	reduce(const Matrix<T, M, N>& matrix, std::size_t row, std::size_t column)
	{
		if (row > M || column > N) {
			throw InvalidDimensionException();
		}

		Matrix<T, M - 1, N - 1> result;

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
	constexpr double det(const Matrix<T, N, N>& matrix)
	{
		double result = 0;
		for (std::size_t i = 0; i < N; ++i) {
			result += std::pow(-1, i + 1) * matrix(1, i) * det(reduce(matrix, 1, i));
		}
		return result;
	}

	template<typename T>
	[[nodiscard]]
	constexpr double det(const Matrix<T, 1, 1>& matrix)
	{
		return matrix(0, 0);
	}

	template<typename T>
	[[nodiscard]]
	constexpr double det(const Matrix<T, 2, 2>& matrix)
	{
		return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
	}

	template<typename T>
	[[nodiscard]]
	constexpr double det(const Matrix<T, 3, 3>& matrix)
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
	constexpr double trace(const Matrix<T, N, N>& matrix)
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
	constexpr Matrix<T, N, M> transpose(const Matrix<T, M, N>& matrix)
	{
		Matrix<T, N, M> result;
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
	constexpr Matrix<T, N, N> inverse(const Matrix<T, N, N>& matrix)
	{
		if (det(matrix) == 0) {
			throw DivideByZeroException();
		}

		Matrix<T, N, N> mat = matrix;
		Matrix<T, N, N> inv = gml::matrix::Id<T, N>;

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
	constexpr Matrix<T, 2, 2> inverse(const Matrix<T, 2, 2>& matrix)
	{
		const double m_det = det(matrix);
		if (m_det == 0) {
			throw DivideByZeroException();
		}

		Matrix<T, 2, 2> inv{
			matrix(1, 1), -matrix(0, 1),
			-matrix(1, 0), matrix(0, 0)
		};
		return inv / m_det;
	}

	template<typename T>
	[[nodiscard]]
	constexpr Matrix<T, 3, 3> inverse(const Matrix<T, 3, 3>& matrix)
	{
		const double m_det = det(matrix);
		if (m_det == 0) {
			throw DivideByZeroException();
		}

		auto& m = matrix;
		Matrix<T, 3, 3> inv{
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

namespace gml::matrix
{
	/**
	 * @brief Constructs a translation matrix.
	 *
	 * @param x the x component
	 * @param y the y component
	 * @param z the z component
	 * @return the translation matrix
	*/
	template<typename T>
	Mat4<T> translate(const T& x, const T& y, const T& z)
	{
		Mat4<T> result = matrix::Id<T, 4>;
		result(0, 3) = x;
		result(1, 3) = y;
		result(2, 3) = z;
		return result;
	}

	/**
	 * @brief Constructs a translation matrix from a vector.
	 *
	 * @param translation the translation vector
	 * @return the translation matrix
	 */
	template<typename T>
	Mat4<T> translate(const Vec3<T>& translation)
	{
		return translate(translation(0), translation(1), translation(2));
	}

	/**
	 * @brief Constructs a scaling matrix.
	 *
	 * @param x the x component
	 * @param y the y component
	 * @param z the z component
	 * @return the scaling matrix
	 */
	template<typename T>
	Mat4<T> scale(const T& x, const T& y, const T& z)
	{
		Mat4<T> result = matrix::Id<T, 4>;
		result(0, 0) = x;
		result(1, 1) = y;
		result(2, 2) = z;
		return result;
	}

	/**
	 * @brief Constructs a scaling matrix from a vector.
	 *
	 * @param scale the scaling vector
	 * @return the scaling matrix
	 */
	template<typename T>
	Mat4<T> scale(const Vec3<T>& scale)
	{
		return scale(scale.x(), scale.y(), scale.z());
	}

	/**
	 * @brief Constructs a rotation matrix from an axis angle
	 * representation.
	 *
	 * @param axis the rotation axis
	 * @param angle the angle in radians
	 * @return the rotation matrix
	 */
	template<typename T>
	Mat4<T> axisAngle(const Vec3<T>& axis, const double angle)
	{
		Vec3<T> a = gml::normalize(axis);
		const T& x = a.x();
		const T& y = a.y();
		const T& z = a.z();
		const T c = std::cos(angle);
		const T s = std::sin(angle);
		const T t = 1 - c;

		Mat4<T> result = matrix::Id<T, 4>;
		result(0, 0) = x * x * t + c;
		result(0, 1) = x * y * t - z * s;
		result(0, 2) = x * z * t + y * s;

		result(1, 0) = y * x * t + z * s;
		result(1, 1) = y * y * t + c;
		result(1, 2) = y * z * t - x * s;

		result(2, 0) = z * x * t - y * s;
		result(2, 1) = z * y * t + x * s;
		result(2, 2) = z * z * t + c;
		return result;
	}


	/**
	 * @brief Constructs a perspective projection matrix.
	 *
	 * @param fov the field of view in degrees
	 * @param aspect the aspect ration
	 * @param near the near plane
	 * @param far the far plane
	 * @return the projection matrix
	 */
	template<typename T>
	Mat4<T> perspective(const double fov, const double aspect, const double near, const double far)
	{
		Mat4<T> result = matrix::Id<T, 4>;
		const double top = near * std::tan(toRad(fov / 2));
		const double bottom = 0 - top;
		const double right = top * aspect;
		const double left = 0 - right;
		result(0, 0) = (2 * near) / (right - left);
		result(0, 2) = (right + left) / (right - left);
		result(1, 1) = (2 * near) / (top - bottom);
		result(1, 2) = (top + bottom) / (top - bottom);
		result(2, 2) = -((far + near) / (far - near));
		result(2, 3) = -((2 * far * near) / (far - near));
		result(3, 2) = -1;
		result(3, 3) = 0;
		return result;
	}

	/**
	 * @brief Constructs an orthographic projection matrix.
	 *
	 * @param left the left plane
	 * @param right the right plane
	 * @param bottom the bottom plane
	 * @param top the top plane
	 * @param near the near plane
	 * @param far the far plane
	 * @return the projection matrix
	 */
	template<typename T>
	Mat4<T> orthographic(
		const double left, const double right,
		const double bottom, const double top,
		const double near, const double far)
	{
		Mat4<T> result = matrix::Id<T, 4>;
		result(0, 0) = 2 / (right - left);
		result(0, 3) = -((right + left) / (right - left));
		result(1, 1) = 2 / (top - bottom);
		result(1, 3) = -((top + bottom) / (top - bottom));
		result(2, 2) = -2 / (far - near);
		result(2, 3) = -((far + near) / (far - near));
		result(3, 3) = 1;
		return result;
	}

	/**
	 * @brief Constructs a lookAt view matrix.
	 *
	 * @param pos the camera position
	 * @param target the view target position
	 * @param up the camera up vector
	 * @return the view matrix
	 */
	template<typename T>
	Mat4<T> lookAt(const Vec3<T>& pos, const Vec3<T>& target, const Vec3<T>& up)
	{
		Vec3<T> direction = normalize(pos - target);
		Vec3<T> right = normalize(cross(up, direction));
		Mat4<T> mat1 = {
			{ right.x(),     right.y(),     right.z(),     0 },
			{ up.x(),        up.y(),        up.z(),        0 },
			{ direction.x(), direction.y(), direction.z(), 0 },
			{ 0,           0,           0,           1 }
		};
		return mat1 * translate<T>(-pos.x(), -pos.y(), -pos.z());
	}

	/**
	 * @brief Constructs a rotation matrix from a quaternion
	 * representation.
	 *
	 * @return the rotation matrix
	 */
	template<typename T>
	constexpr Mat4<T> fromQuaternion(const Quaternion <T>& quaternion)
	{
		const Quaternion <T> q = normalize(quaternion);

		Mat4<T> result = matrix::Id<T, 4>;
		result(0, 0) = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
		result(0, 1) = 2 * q.x * q.y - 2 * q.z * q.w;
		result(0, 2) = 2 * q.x * q.z + 2 * q.y * q.w;

		result(1, 0) = 2 * q.x * q.y + 2 * q.z * q.w;
		result(1, 1) = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
		result(1, 2) = 2 * q.y * q.z - 2 * q.x * q.w;

		result(2, 0) = 2 * q.x * q.z - 2 * q.y * q.w;
		result(2, 1) = 2 * q.y * q.z + 2 * q.x * q.w;
		result(2, 2) = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
		return result;
	}
}

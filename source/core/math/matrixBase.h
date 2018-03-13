#pragma once

#include <array>
#include <initializer_list>
#include <ostream>

#include "maths.h"
#include "matrixSpecial.h"
#include "vectorbase.h"

namespace gml
{
	/**
	 * @brief Represents a generic NxM dimensional matrix 
	 * 
	 * @tparam T the generic type
	 * @tparam Rows the number of rows
	 * @tparam Columns the number of columns
	 */
	template <typename T, size_t Rows, size_t Columns>
	class MatrixBase
	{
	private:
		/**
		 * @brief The matrix fields as a 2d array.
		 */
		std::array<
			std::array<T, (Columns > 0) ? Columns : 1>,
			(Rows > 0) ? Rows : 1> data;

	public:
		/**
		 * @brief Initializes the diagonal values with ones, the rest with zeros.
		 */
		MatrixBase();

		/**
		* @brief Initializes all fields with the same value.
		*/
		explicit MatrixBase(const T& value);

		/**
		 * @brief Initializes the fields by a 1d initialzer list.
		 * 
		 * @param init the initializer list
		 */
		MatrixBase(const std::initializer_list<T>& init);

		/**
		* @brief Initializes the fields by a 2d initialzer list.
		*
		* @param init the initializer list
		*/
		MatrixBase(const std::initializer_list<std::initializer_list<T>>& init);

		/**
		 * @brief Initialzes this matrix with values from a linear c-style array.
		 * 
		 * @param arr the array
		 */
		explicit MatrixBase(const T arr[]);

	public:
		/**
		 * @brief Returns a reference to this matrix's field at a given position.
		 * 
		 * @param row the row
		 * @param column the coulmn
		 * @return a reference to the field
		 */
		T& at(size_t row, size_t column);

		/**
		* @brief Returns a const reference to this matrix's field at a given 
		* position.
		*
		* @param row the row
		* @param column the coulmn
		* @return a const reference to the field
		*/
		const T& at(size_t row, size_t column) const;

		/**
		* @brief Returns the determinante of this matrix.
		*
		* @return the determinante
		*/
		double det() const;

		/**
		 * @brief Fills the given c-style array with values from this matrix.
		 * 
		 * @param arr the array to fill
		 */
		void convertToArray(T arr[]) const;

		MatrixBase<T, Rows, Columns>& operator+=(
			const MatrixBase<T, Rows, Columns>& right);
		MatrixBase<T, Rows, Columns>& operator-=(
			const MatrixBase<T, Rows, Columns>& right);
		MatrixBase<T, Rows, Columns>& operator*=(const T& right);
		MatrixBase<T, Rows, Columns>& operator/=(const T& right);

		template<typename U, size_t N>
		friend MatrixBase<U, N, N> inverse(
			const MatrixBase<U, N, N>& matrix);

	private:
		/**
		* @brief Returns a matrix with a given row and column removed.
		*
		* @param row the row to remove
		* @param column the column to remove
		* @return the stripped matrix
		*
		* @throws InvalidDimensionException if row or column is out of bounds
		*/
		MatrixBase<T, Rows - 1, Columns - 1> reduce(size_t row, size_t column)
			const;

		/**
		* @brief Adds a row to another row in this matrix.
		*
		* @param first the row to add the second row
		* @param second the row the first row will be added to
		*/
		void addRows(size_t first, size_t second);

		/**
		* @brief Adds a muliplied row to another mulitplied row in this matrix.
		*
		* @param first the row to add the second row
		* @param firstFactor factor for the first row
		* @param second the row the first row will be added to
		* @param secondFactor factor for the second row
		*/
		void addMultRows(
			size_t first, double firstFactor,
			size_t second, double secondFactor);

		/**
		* @brief Muliplies a row by the given factor.
		*
		* @param row the row to modify
		* @param factor the factor
		*/
		void multRow(size_t row, double factor);

		/**
		* @brief Switch two rows in this matrix.
		*
		* @param first the first row to switch
		* @param second the second row to switch
		*/
		void switchRows(size_t first, size_t second);
	};

	/**
	* @brief Transposes a given matrix.
	*
	* @tparam T the type
	* @tparam Rows rows of the input matrix
	* @tparam Columns columns of the input matrix
	* @param matrix the matrix to transpose
	* @return the transposed matrix
	*/
	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Columns, Rows> transpose(
		const MatrixBase<T, Rows, Columns>& matrix);

	/**
	* @brief Calculates the inverse of a given matrix.
	*
	* @tparam T the type
	* @tparam Rows rows of the input matrix
	* @param matrix the matrix to invert
	* @return the inverse
	*
	* @throws InvalidDimensionException if the matrix is not quadratic
	* @throws DivideByZeroException if the determinante is zero
	*/
	template <typename T, size_t Rows>
	MatrixBase<T, Rows, Rows> inverse(
		const MatrixBase<T, Rows, Rows>& matrix);


	template <typename T, size_t Rows, size_t Columns>
	std::ostream& operator<<(
		std::ostream& os,
		const MatrixBase<T, Rows, Columns>& matrix);

	template <typename T, size_t Rows, size_t Columns>
	bool operator==(
		const MatrixBase<T, Rows, Columns>& left,
		const MatrixBase<T, Rows, Columns>& right);

	template <typename T, size_t Rows, size_t Columns>
	bool operator!=(
		const MatrixBase<T, Rows, Columns>& left,
		const MatrixBase<T, Rows, Columns>& right);

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator+(
		const MatrixBase<T, Rows, Columns>& left,
		const MatrixBase<T, Rows, Columns>& right);

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator-(
		const MatrixBase<T, Rows, Columns>& left,
		const MatrixBase<T, Rows, Columns>& right);

	template <typename T, size_t RowsL, size_t ColumnsR, size_t RowCol>
	MatrixBase<T, RowsL, ColumnsR> operator*(
		const MatrixBase<T, RowsL, RowCol>& left,
		const MatrixBase<T, RowCol, ColumnsR>& right);

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator*(
		const T & left,
		const MatrixBase<T, Rows, Columns>& right);

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator*(
		const MatrixBase<T, Rows, Columns>& left,
		const T & right);

	template <typename T, size_t Rows, size_t Columns>
	VectorBase<T, Rows> operator*(
		const MatrixBase<T, Rows, Columns>& left,
		const VectorBase<T, Columns>& right);

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator/(
		const MatrixBase<T, Rows, Columns>& left,
		const T & right);
}

#include "matrixBase.tpp"

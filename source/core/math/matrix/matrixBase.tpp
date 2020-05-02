#pragma once

namespace gml
{
	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows - 1, Columns - 1> MatrixBase<T, Rows, Columns>::reduce(
		const size_t row,
		const size_t column) const
	{
		if (row > Rows || column > Columns)
		{
			throw InvalidDimensionException();
		}

		MatrixBase<T, Rows - 1, Columns - 1> result;
		for (size_t i = 0; i < Rows - 1; ++i)
		{
			const auto rowI = (i < row) ? i : i + 1;
			for (size_t j = 0; j < Columns - 1; ++j)
			{
				const auto colI = (j < column) ? j : j + 1;
				result.at(i, j) = this->at(rowI, colI);
			}
		}
		return result;
	}

	template <typename T, size_t Rows, size_t Columns>
	void MatrixBase<T, Rows, Columns>::addRows(
		const size_t first,
		const size_t second)
	{
		for (size_t i = 0; i < Columns; ++i)
		{
			this->at(second, i) += this->at(first, i);
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	void MatrixBase<T, Rows, Columns>::addMultRows(
		const size_t first, const double firstFactor,
		const size_t second, const double secondFactor)
	{
		for (size_t i = 0; i < Columns; ++i)
		{
			this->at(second, i) =
				at(second, i) * secondFactor + at(first, i) * firstFactor;
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	void MatrixBase<T, Rows, Columns>::multRow(
		const size_t row,
		const double factor)
	{
		for (size_t i = 0; i < Columns; ++i)
		{
			this->at(row, i) *= factor;
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	void MatrixBase<T, Rows, Columns>::switchRows(
		const size_t first,
		const size_t second)
	{
		for (size_t i = 0; i < Columns; i++)
		{
			T temp = at(first, i);
			at(first, i) = at(second, i);
			at(second, i) = temp;
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>::MatrixBase()
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				if (i == j)
				{
					data.at(i).at(j) = 1.0f;
				}
				else
				{
					data.at(i).at(j) = 0.0f;
				}
			}
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>::MatrixBase(
		const T& value)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			data.at(i).fill(value);
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>::MatrixBase(
		const std::initializer_list<T>& init)
	{
		if (init.size() != Rows * Columns)
		{
			throw InvalidDimensionException();
		}

		size_t i = 0;
		for (const auto& element : init)
		{
			const auto row = static_cast<int>(i / Columns);
			const auto col = i % Columns;
			data.at(row).at(col) = element;
			++i;
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>::MatrixBase(
		const std::initializer_list<std::initializer_list<T>>& init)
	{
		if (init.size() != Rows || init.begin()->size() != Columns)
		{
			throw InvalidDimensionException();
		}

		size_t i = 0, j = 0;
		for (const auto& list : init)
		{
			for (const auto& element : list)
			{
				data.at(i).at(j) = element;
				++j;
			}
			j = 0;
			++i;
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>::MatrixBase(const T arr[])
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				data.at(i).at(j) = arr[i * Columns + j];
			}
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>::MatrixBase(const MatrixBase<T, Rows, Columns>& other)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) = other.at(i, j);
			}
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	template <typename T2>
	MatrixBase<T, Rows, Columns>::MatrixBase(const MatrixBase<T2, Rows, Columns>& other)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) = other.at(i, j);
			}
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>& MatrixBase<T, Rows, Columns>::operator=(const MatrixBase<T, Rows, Columns>& other)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) = other.at(i, j);
			}
		}
		return *this;
	}

	template <typename T, size_t Rows, size_t Columns>
	template<typename T2>
	MatrixBase<T, Rows, Columns>& MatrixBase<T, Rows, Columns>::operator=(const MatrixBase<T2, Rows, Columns>& other)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) = other.at(i, j);
			}
		}
		return *this;
	}

	template <typename T, size_t Rows, size_t Columns>
	T& MatrixBase<T, Rows, Columns>::at(size_t row, size_t column)
	{
		return data.at(row).at(column);
	}

	template <typename T, size_t Rows, size_t Columns>
	const T& MatrixBase<T, Rows, Columns>::at(size_t row, size_t column) const
	{
		return data.at(row).at(column);
	}

	template <typename T, size_t Rows, size_t Columns>
	double MatrixBase<T, Rows, Columns>::det() const
	{
		if (Rows == 1 && Columns == 1)
		{
			return this->at(0, 0);
		}

		double det = 0;
		for (size_t i = 0; i < Columns; ++i)
		{
			det += std::pow(-1, i + 1) * at(1, i) * reduce(1, i).det();
		}
		return det;
	}

	template <typename T, size_t Rows, size_t Columns>
	double MatrixBase<T, Rows, Columns>::trace() const
	{
		double trace = 0;
		for(size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				if (i == j)
				{
					trace += this->at(i, j);
				}
			}
		}
		return trace;
	}

	template <typename T, size_t Rows, size_t Columns>
	void MatrixBase<T, Rows, Columns>::convertToArray(T arr[]) const
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				arr[i * Columns + j] = this->at(i, j);
			}
		}
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>& MatrixBase<T, Rows, Columns>::operator+=(const MatrixBase<T, Rows, Columns>& right)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) += right.at(i, j);
			}
		}
		return *this;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>& MatrixBase<T, Rows, Columns>::operator-=(const MatrixBase<T, Rows, Columns>& right)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) -= right.at(i, j);
			}
		}
		return *this;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>& MatrixBase<T, Rows, Columns>::operator*=(const T& right)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) *= right;
			}
		}
		return *this;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns>& MatrixBase<T, Rows, Columns>::operator/=(const T& right)
	{
		if (right == 0)
		{
			throw DivideByZeroException();
		}

		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				this->at(i, j) /= right;
			}
		}
		return *this;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Columns, Rows> transpose(const MatrixBase<T, Rows, Columns>& matrix)
	{
		MatrixBase<T, Columns, Rows> result;
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				result.at(j, i) = matrix.at(i, j);
			}
		}
		return result;
	}

	template <typename T, size_t Rows>
	MatrixBase<T, Rows, Rows> inverse(const MatrixBase<T, Rows, Rows>& matrix)
	{
		if (matrix.det() == 0)
		{
			throw DivideByZeroException();
		}

		MatrixBase<T, Rows, Rows> inverse;
		MatrixBase<T, Rows, Rows> mat = matrix;

		// Zeilen vertauschen falls erster Eintrag 0 ist
		if (mat.at(0, 0) == 0)
		{
			for (size_t i = 0; i < Rows; ++i)
			{
				if (mat.at(i, 0) != 0)
				{
					mat.switchRows(0, i);
					inverse.switchRows(0, i);
					break;
				}
			}
		}

		// Diagonale auf 1 und Werte unterhalb auf 0 bringen
		for (size_t i = 0; i < Rows; ++i)
		{
			// Wert der Diagonalen auf 1 bringen
			T value = mat.at(i, i);
			mat.multRow(i, 1 / value);
			inverse.multRow(i, 1 / value);

			// Werte unterhalb der Diagonalen auf 0 bringen
			for (size_t j = i + 1; j < Rows; ++j)
			{
				T firstFactor = -mat.at(j, i);
				T secondFactor = mat.at(i, i);

				mat.addMultRows(i, firstFactor, j, secondFactor);
				inverse.addMultRows(i, firstFactor, j, secondFactor);
			}
		}
		// Werte oberhalb der Diagonalen auf 1 bringen
		for (size_t i = Rows - 1; i > 0; --i)
		{
			for (size_t j = i - 1; j >= 0; --j)
			{
				T firstFactor = -mat.at(j, i);
				T secondFactor = 1;

				mat.addMultRows(i, firstFactor, j, secondFactor);
				inverse.addMultRows(i, firstFactor, j, secondFactor);
			}
		}
		return inverse;
	}

	template <typename T, size_t Rows, size_t Columns>
	std::ostream& operator<<(std::ostream& os, const MatrixBase<T, Rows, Columns>& matrix)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				os << matrix.at(i, j) << " ";
			}
			os << "\n";
		}
		return os;
	}

	template <typename T, size_t Rows, size_t Columns>
	bool operator==(const MatrixBase<T, Rows, Columns>& left, const MatrixBase<T, Rows, Columns>& right)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				if (left.at(i, j) != right.at(i, j))
					return false;
			}
		}
		return true;
	}

	template <typename T, size_t Rows, size_t Columns>
	bool operator!=(const MatrixBase<T, Rows, Columns>& left, const MatrixBase<T, Rows, Columns>& right)
	{
		return !(left == right);
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator+(
		const MatrixBase<T, Rows, Columns>& left,
		const MatrixBase<T, Rows, Columns>& right)
	{
		return MatrixBase<T, Rows, Columns>(left) += right;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator-(
		const MatrixBase<T, Rows, Columns>& left,
		const MatrixBase<T, Rows, Columns>& right)
	{
		return MatrixBase<T, Rows, Columns>(left) -= right;
	}

	template <typename T, size_t RowsL, size_t ColumnsR, size_t RowCol>
	MatrixBase<T, RowsL, ColumnsR> operator*(
		const MatrixBase<T, RowsL, RowCol>& left,
		const MatrixBase<T, RowCol, ColumnsR>& right)
	{
		MatrixBase<T, RowsL, ColumnsR> result;
		for (size_t i = 0; i < RowsL; ++i)
		{
			for (size_t j = 0; j < ColumnsR; ++j)
			{
				T value = 0;
				for (size_t k = 0; k < RowCol; ++k)
				{
					value += left.at(i, k) * right.at(k, j);
				}
				result.at(i, j) = value;
			}
		}
		return result;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator*(
		const T & left,
		const MatrixBase<T, Rows, Columns>& right)
	{
		return MatrixBase<T, Rows, Columns>(right) *= left;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator*(
		const MatrixBase<T, Rows, Columns>& left,
		const T & right)
	{
		return MatrixBase<T, Rows, Columns>(left) *= right;
	}

	template <typename T, size_t Rows, size_t Columns>
	VectorBase<T, Rows> operator*(
		const MatrixBase<T, Rows, Columns>& left,
		const VectorBase<T, Columns>& right)
	{
		VectorBase<T, Rows> result;
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Columns; ++j)
			{
				result.at(i) += left.at(i, j) * right.at(j);
			}
		}
		return result;
	}

	template <typename T, size_t Rows, size_t Columns>
	MatrixBase<T, Rows, Columns> operator/(
		const MatrixBase<T, Rows, Columns>& left,
		const T & right)
	{
		return MatrixBase<T, Rows, Columns>(left) /= right;
	}
}

#pragma once

namespace gml
{
	template <typename T, size_t Rows, size_t Columns>
	class MatrixBase;

	/**
	* @brief Represents a specialized 1x1 matrix.
	*
	* @tparam T the type
	*/
	template<typename T>
	class MatrixBase<T, 1, 1>
	{
	private:
		T data;

	public:
		double det() const
		{
			return data;
		}

		double trace() const
		{
			return data;
		}

		T& at(size_t row = 0, size_t column = 0)
		{
			return data;
		}

		const T& at(size_t row = 0, size_t column = 0) const
		{
			return data;
		}
	};

	/* 0x0 matrices are not allowed */
	template<typename T>
	class MatrixBase<T, 0, 0>
	{
	};

	template<typename T, size_t Rows>
	class MatrixBase<T, Rows, 0>
	{
	};

	template<typename T, size_t Columns>
	class MatrixBase<T, 0, Columns>
	{
	};
}
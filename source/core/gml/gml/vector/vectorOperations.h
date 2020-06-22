#pragma once

#include "vectorBase.h"

namespace gml
{
	/**
	 * @brief Calculates the dot product of two vectors.
	 *
	 * @tparam T The vectors' data type.
	 * @tparam Size The vectors' dimension.
	 * @param lhs The first vector.
	 * @param rhs The second vector.
	 * @return The dot product.
	 */
	template<typename T, std::size_t Size>
	[[nodiscard]]
	constexpr T dot(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs)
	{
		T result = 0;
		for (std::size_t i = 0; i < Size; ++i) {
			result += lhs(i) * rhs(i);
		}
		return result;
	}

	/**
	 * @brief Returns the squared euclidean length of a vector.
	 *
	 * @return The squared euclidean length of the vector.
	 */
	template<typename T, std::size_t Size>
	[[nodiscard]]
	constexpr double sqrLength(const VectorBase<T, Size>& vector)
	{
		return dot(vector, vector);
	}

	/**
	 * @brief Returns the euclidean length of a vector.
	 *
	 * @return The euclidean length of the vector.
	 */
	template<typename T, std::size_t Size>
	[[nodiscard]]
	constexpr double length(const VectorBase<T, Size>& vector)
	{
		return std::sqrt(sqrLength(vector));
	}

	/**
	 * @brief Normalizes a vector.
	 *
	 * @return A normalized copy of the vector.
	 *
	 * @throws DivideByZeroException The vector's length is zero.
	 */
	template<typename T, std::size_t Size>
	[[nodiscard]]
	constexpr VectorBase<T, Size> normalize(const VectorBase<T, Size>& vector)
	{
		return vector / length(vector);
	}

	/**
	 * @brief Calculates the angle between two vectors.
	 *
	 * @tparam T The vectors' data type.
	 * @tparam Size The vectors' dimension.
	 * @param lhs The first vector.
	 * @param rhs The second vector.
	 * @return The angle in radians.
	 */
	template<typename T, std::size_t Size>
	[[nodiscard]]
	constexpr double angle(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs)
	{
		const double lengths = length(lhs) * length(rhs);
		if (lengths == 0.0) {
			throw DivideByZeroException();
		}
		return std::acos(dot(lhs, rhs) / lengths);
	}
}
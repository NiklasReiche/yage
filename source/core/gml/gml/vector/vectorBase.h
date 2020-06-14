#pragma once

#include <array>
#include <initializer_list>
#include <ostream>

#include "../constraints.h"
#include "../exception.h"
#include "../maths.h"

namespace gml
{
	/**
	 * @brief Represents a generic n-dimensional vector.
	 * 
	 * @tparam T The type of the vector's components.
	 * @tparam Size The vector's dimension.
	 */
	template<typename T, std::size_t Size>
	requires StrictlyPositive<Size>
	class VectorBase
	{
	public:
		/**
		 * @brief Value-initializes all vector components.
		 */
		constexpr VectorBase()
			: elements{ }
		{
		}

		/**
		 * @brief Initializes all vector components with a given value.
		 * 
		 * @param value The initializing value.
		 */
		constexpr explicit VectorBase(T value)
		{
			elements.fill(value);
		}

		/**
		* @brief Initializes the fields by an initializer list.
		*
		* @param init the initializer list
		* 
		* @throws InvalidDimensionException if initializer list is different size
		* from this vector's dimension
		*/
		constexpr VectorBase(const std::initializer_list<T>& init)
		{
			if (init.size() != Size) {
				throw InvalidDimensionException();
			}
			std::copy(std::begin(init), std::end(init), std::begin(elements));
		}

		/**
		 * @brief Constructs a vector by copy-converting the components of another vector.
		 *
		 * Type T of the constructed vector must to be assignable from type T2 of the vector being copied.
		 *
		 * @tparam T2 Type of the components of the vector from which to convert.
		 * @param other Vector from which to copy and convert the components.
		 */
		template<typename T2>
		constexpr VectorBase(const VectorBase<T2, Size>& other)
		{
			for (std::size_t i = 0; i < Size; ++i) {
				elements.at(i) = other(i);
			}
		}

		/**
		 * @brief Assigns this vector by copy-converting the components of another vector.
		 *
		 * Type T of the constructed vector must to be assignable from type T2 of the copied vector.
		 *
		 * @tparam T2 Type of the components of the vector from which to convert.
		 * @param other Vector from which to copy and convert the components.
		 * @return This vector.
		 */
		template<typename T2>
		constexpr VectorBase<T, Size>& operator=(const VectorBase<T2, Size>& other)
		{
			for (std::size_t i = 0; i < Size; ++i) {
				elements.at(i) = other(i);
			}
			return *this;
		}

		/**
		 * @brief Returns a reference to this vector's component at a given index.
		 *
		 * @param index The components's index.
		 * @return A reference to the component.
		 */
		constexpr T& operator()(int i)
		{
			return elements.at(i);
		}

		/**
		 * @brief Returns a const reference to this vector's component at a given index.
		 *
		 * @param pos The components's index.
		 * @return A const reference to the component.
		 */
		constexpr const T& operator()(int i) const
		{
			return elements.at(i);
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
		 * @brief Normalizes this vector.
		 * 
		 * @return This vector.
		 * @throws DivideByZeroException This vector's length is zero.
		 */
		constexpr VectorBase<T, Size>& normalize();

		/**
		 * @brief Unary component-wise negation.
		 * @return A negated vector.
		 */
		constexpr VectorBase<T, Size> operator-() const
		{
			VectorBase<T, Size> result;
			for (std::size_t i = 0; i < Size; ++i) {
				result(i) = -elements.at(i);
			}
			return result;
		}

		/**
		 * @brief Component-wise addition.
		 * @param right The vector to add to this vector.
		 * @return This vector.
		 */
		constexpr VectorBase<T, Size>& operator+=(const VectorBase<T, Size>& right)
		{
			for (std::size_t i = 0; i < Size; ++i) {
				elements.at(i) += right(i);
			}
			return *this;
		}

		/**
		 * @brief Component-wise subtraction.
		 * @param right The vector to subtract from this vector.
		 * @return This vector.
		 */
		constexpr VectorBase<T, Size>& operator-=(const VectorBase<T, Size>& right)
		{
			for (std::size_t i = 0; i < Size; ++i) {
				elements.at(i) -= right(i);
			}
			return *this;
		}

		/**
		 * @brief Component-wise scalar multiplication.
		 * @tparam T2 The scalar's type.
		 * @param right The scalar by which to multiply this vector.
		 * @return This vector.
		 */
		constexpr VectorBase<T, Size>& operator*=(const T& right)
		{
			for (std::size_t i = 0; i < Size; ++i) {
				elements.at(i) *= right;
			}
			return *this;
		}

		/**
		 * @brief Component-wise scalar division.
		 * @tparam T2 The scalar's type.
		 * @param right The scalar by which to divide this vector.
		 * @return This vector.
		 * @throws DivideByZeroException The scalar is zero.
		 */
		constexpr VectorBase<T, Size>& operator/=(const T& right)
		{
			if (right == 0) {
				throw DivideByZeroException();
			}

			for (std::size_t i = 0; i < Size; ++i) {
				elements.at(i) /= right;
			}
			return *this;
		}

	private:
		/**
		* @brief The vector's components.
		*/
		std::array<T, Size> elements;
	};

	template<typename T, std::size_t Size>
	[[nodiscard]]
	constexpr T dot(const VectorBase<T, Size>& left, const VectorBase<T, Size>& right)
	{
		T result = 0;
		for (std::size_t i = 0; i < Size; ++i) {
			result += left(i) * right(i);
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

	template<typename T, std::size_t Size>
	std::ostream& operator<<(std::ostream& os, const VectorBase<T, Size>& vec)
	{
		os << "[";
		for (std::size_t i = 0; i < Size; ++i) {
			os << vec(i) << (i == Size - 1 ? "" : " ");
		}
		os << "]";
		return os;
	}

	template<typename T, std::size_t Size>
	constexpr bool operator==(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs)
	{
		for (std::size_t i = 0; i < Size; ++i) {
			if (lhs(i) != rhs(i)) {
				return false;
			}
		}
		return true;
	}

	template<typename T, std::size_t Size>
	constexpr bool operator!=(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, std::size_t Size>
	constexpr VectorBase<T, Size> operator+(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs)
	{
		return VectorBase<T, Size>(lhs) += rhs;
	}

	template<typename T, std::size_t Size>
	constexpr VectorBase<T, Size> operator-(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs)
	{
		return VectorBase<T, Size>(lhs) -= rhs;
	}

	template<typename T, std::size_t Size>
	constexpr VectorBase<T, Size> operator*(const double lhs, const VectorBase<T, Size>& rhs)
	{
		return VectorBase<T, Size>(rhs) *= lhs;
	}

	template<typename T, std::size_t Size>
	constexpr VectorBase<T, Size> operator*(const VectorBase<T, Size>& lhs, const double rhs)
	{
		return VectorBase<T, Size>(lhs) *= rhs;
	}

	template<typename T, std::size_t Size>
	constexpr VectorBase<T, Size> operator/(const VectorBase<T, Size>& lhs, const double rhs)
	{
		return VectorBase<T, Size>(lhs) /= rhs;
	}

	//-------------------------------------------------------------------------------------------

	template<typename T, std::size_t Size>
	requires StrictlyPositive<Size>
	constexpr VectorBase<T, Size>& VectorBase<T, Size>::normalize()
	{
		return operator=(gml::normalize(*this));
	}
}

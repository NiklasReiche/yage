#pragma once

#include <array>
#include <initializer_list>
#include <ostream>

#include "../constraints.h"
#include "../exception.h"
#include "../maths.h"

namespace gml
{
	template<typename T, std::size_t Size> requires StrictlyPositive<Size>
	class VectorBase;

	template<typename T, std::size_t Size>
	constexpr double length(const VectorBase <T, Size>& vector);

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
		 * @brief Initializes the components from a continuous memory block.
		 *
		 * @param data Continuous memory to copy elements from.
		 */
		constexpr explicit VectorBase(std::span<T, Size> data)
		{
			std::copy(data.begin(), data.end(), elements.begin());
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
		 * @param pos The component's index.
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
		[[nodiscard]]
		constexpr T* data() noexcept
		{
			return elements.data();
		}

		/**
		 * @brief Gives direct access to the underlying array.
		 *
		 * @return Pointer to the underlying element storage (equal to the address of the first element).
		 */
		[[nodiscard]]
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
		constexpr VectorBase<T, Size>& normalize()
		{
			return operator/=(length(*this));
		}

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
}

#pragma once

#include <array>
#include <initializer_list>
#include <ostream>
#include <span>

#include "constraints.h"
#include "exception.h"
#include "maths.h"

namespace gml
{
	template<typename T, std::size_t Size> requires StrictlyPositive<Size>
	class Vector;

    template<std::size_t Size> requires StrictlyPositive<Size>
    using Veci = Vector<int, Size>;
    template<std::size_t Size> requires StrictlyPositive<Size>
    using Vecf = Vector<float, Size>;
    template<std::size_t Size> requires StrictlyPositive<Size>
    using Vecd = Vector<double, Size>;

	template<typename T>
	using Vec2 = Vector<T, 2>;

	using Vec2i = Vec2<int>;
    using Vec2ui = Vec2<unsigned int>;
	using Vec2f = Vec2<float>;
	using Vec2d = Vec2<double>;

	template<typename T>
	using Vec3 = Vector<T, 3>;

	using Vec3i = Vec3<int>;
    using Vec3ui = Vec3<unsigned int>;
	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;

	template<typename T>
	using Vec4 = Vector<T, 4>;

	using Vec4i = Vec4<int>;
    using Vec4ui = Vec4<unsigned int>;
	using Vec4f = Vec4<float>;
	using Vec4d = Vec4<double>;

	template<typename T, std::size_t Size>
	constexpr double length(const Vector<T, Size>& vector);

	/**
	 * @brief Represents a generic n-dimensional vector.
	 * 
	 * @tparam T The type of the vector's components.
	 * @tparam Size The vector's dimension.
	 */
	template<typename T, std::size_t Size> requires StrictlyPositive<Size>
	class Vector
	{
	public:
		/**
		 * @brief Value-initializes all vector components.
		 */
		constexpr Vector()
			: elements{ }
		{
		}

		/**
		 * @brief Initializes all vector components with a given value.
		 * 
		 * @param value The initializing value.
		 */
		constexpr explicit Vector(T value)
		{
			elements.fill(value);
		}

		constexpr Vector(T x, T y) requires (Size == 2)
			: elements { x, y }
		{
		}

		constexpr Vector(T x, T y, T z) requires (Size == 3)
			: elements{ x, y, z }
		{
		}

		constexpr Vector(T x, T y, T z, T w) requires (Size == 4)
			: elements{ x, y, z, w }
		{
		}

		/**
		 * @brief Initializes the components from a continuous memory block.
		 *
		 * @param data Continuous memory to copy elements from.
		 */
		constexpr explicit Vector(std::span<T, Size> data)
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
		constexpr Vector(const std::initializer_list<T>& init)
		{
            // TODO: if we take init as an array we can have compile-time size correctness
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
		constexpr Vector(const Vector<T2, Size>& other)
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
		constexpr Vector<T, Size>& operator=(const Vector<T2, Size>& other)
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

		[[nodiscard]]
		constexpr T& x() requires (Size >= 1 && Size <= 4)
		{
			return (*this)(0);
		}

		[[nodiscard]]
		constexpr const T& x() const requires (Size >= 1 && Size <= 4)
		{
			return (*this)(0);
		}

		[[nodiscard]]
		constexpr T& y() requires (Size >= 2 && Size <= 4)
		{
			return (*this)(1);
		}

		[[nodiscard]]
		constexpr const T& y() const requires (Size >= 2 && Size <= 4)
		{
			return (*this)(1);
		}

		[[nodiscard]]
		constexpr T& z() requires (Size >= 3 && Size <= 4)
		{
			return (*this)(2);
		}

		[[nodiscard]]
		constexpr const T& z() const requires (Size >= 3 && Size <= 4)
		{
			return (*this)(2);
		}

		[[nodiscard]]
		constexpr T& w() requires (Size == 4)
		{
			return (*this)(3);
		}

		[[nodiscard]]
		constexpr const T& w() const requires (Size == 4)
		{
			return (*this)(3);
		}

		/**
		 * @brief Normalizes this vector.
		 * 
		 * @return This vector.
		 * @throws DivideByZeroException This vector's length is zero.
		 */
		constexpr Vector<T, Size>& normalize()
		{
			return operator/=(length(*this));
		}

		/**
		 * @brief Unary component-wise negation.
		 * @return A negated vector.
		 */
		constexpr Vector<T, Size> operator-() const
		{
			Vector<T, Size> result;
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
		constexpr Vector<T, Size>& operator+=(const Vector<T, Size>& right)
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
		constexpr Vector<T, Size>& operator-=(const Vector<T, Size>& right)
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
		constexpr Vector<T, Size>& operator*=(const T& right)
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
		constexpr Vector<T, Size>& operator/=(const T& right)
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
	std::ostream& operator<<(std::ostream& os, const Vector<T, Size>& vec)
	{
		os << "[";
		for (std::size_t i = 0; i < Size; ++i) {
			os << vec(i) << (i == Size - 1 ? "" : " ");
		}
		os << "]";
		return os;
	}

	template<typename T, std::size_t Size>
	constexpr bool operator==(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
	{
		for (std::size_t i = 0; i < Size; ++i) {
			if (lhs(i) != rhs(i)) {
				return false;
			}
		}
		return true;
	}

	template<typename T, std::size_t Size>
	constexpr bool operator!=(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, std::size_t Size>
	constexpr Vector<T, Size> operator+(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
	{
		return Vector<T, Size>(lhs) += rhs;
	}

	template<typename T, std::size_t Size>
	constexpr Vector<T, Size> operator-(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
	{
		return Vector<T, Size>(lhs) -= rhs;
	}

	template<typename T, std::size_t Size>
	constexpr Vector<T, Size> operator*(const double lhs, const Vector<T, Size>& rhs)
	{
		return Vector<T, Size>(rhs) *= lhs;
	}

	template<typename T, std::size_t Size>
	constexpr Vector<T, Size> operator*(const Vector<T, Size>& lhs, const double rhs)
	{
		return Vector<T, Size>(lhs) *= rhs;
	}

	template<typename T, std::size_t Size>
	constexpr Vector<T, Size> operator/(const Vector<T, Size>& lhs, const double rhs)
	{
		return Vector<T, Size>(lhs) /= rhs;
	}

	/**
	* @brief Calculates the cross product of two 3-dimensional vectors.
	*
	* @tparam T the type
	* @param left the first vector
	* @param right the second vector
	* @return the cross product
	*/
	template<typename T>
	constexpr Vector<T, 3> cross(Vector<T, 3> left, Vector<T, 3> right)
	{
		Vector<T, 3> result;
		result.x() = left.y() * right.z() - left.z() * right.y();
		result.y() = left.z() * right.x() - left.x() * right.z();
		result.z() = left.x() * right.y() - left.y() * right.x();
		return result;
	}

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
	constexpr T dot(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
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
	constexpr double sqrLength(const Vector<T, Size>& vector)
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
	constexpr double length(const Vector<T, Size>& vector)
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
	constexpr Vector<T, Size> normalize(const Vector<T, Size>& vector)
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
	constexpr double angle(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
	{
		const double lengths = length(lhs) * length(rhs);
		if (lengths == 0.0) {
			throw DivideByZeroException();
		}
		return std::acos(dot(lhs, rhs) / lengths);
	}
}

namespace gml::vector
{
	template<typename T>
	constexpr Vec3<T> worldForward()
	{
		return Vec3<T>(0, 0, 1);
	}

	template<typename T>
	constexpr Vec3<T> worldUp()
	{
		return Vec3<T>(0, 1, 0);
	}

	template<typename T>
	constexpr Vec3<T> worldRight()
	{
		return Vec3<T>(-1, 0, 0); // TODO
	}
}

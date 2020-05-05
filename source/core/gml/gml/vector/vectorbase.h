#pragma once

#include <array>
#include <initializer_list>
#include <ostream>

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
	template<typename T, size_t Size>
	class VectorBase
	{
	public:
		/**
		 * @brief Value-initializes all vector components.
		 */
		VectorBase();

		/**
		 * @brief Initializes all vector components with a given value.
		 * 
		 * @param value The initializing value.
		 */
		explicit VectorBase(T value);

		/**
		* @brief Initializes the fields by an initializer list.
		*
		* @param init the initializer list
		* 
		* @throws InvalidDimensionException if initializer list is different size
		* from this vector's dimension
		*/
		VectorBase(const std::initializer_list<T> &init);

		/**
		 * @brief Constructs a vector by copy-converting the components of another vector.
		 *
		 * Type T of the constructed vector must to be assignable from type T2 of the vector being copied.
		 *
		 * @tparam T2 Type of the components of the vector from which to convert.
		 * @param other Vector from which to copy and convert the components.
		 */
		template<typename T2>
		VectorBase(const VectorBase<T2, Size> &other);

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
		VectorBase<T, Size> &operator=(const VectorBase<T2, Size> &other);

		/**
		 * @brief Returns a reference to this vector's component at a given index.
		 * 
		 * @param index The components's index.
		 * @return A reference to the component.
		 */
		T &at(size_t index);

		/**
		 * @brief Returns a const reference to this vector's component at a given index.
		 * 
		 * @param pos The components's index.
		 * @return A const reference to the component.
		 */
		const T &at(size_t pos) const;

		/**
		 * @brief Returns the squared euclidean length of this vector.
		 * 
		 * @return The squared euclidean length of this vector.
		 */
		[[nodiscard]]
		double sqrLength() const;

		/**
		 * @brief Returns the euclidean length of this vector.
		 * 
		 * @return The euclidean length of this vector.
		 */
		[[nodiscard]]
		double length() const;

		/**
		 * @brief Normalizes this vector.
		 * 
		 * @return This vector.
		 * @throws DivideByZeroException This vector's length is zero.
		 */
		VectorBase<T, Size> &normalize();

		/**
		 * @brief Unary component-wise negation.
		 * @return A negated vector.
		 */
		VectorBase<T, Size> operator-() const;

		/**
		 * @brief Component-wise addition.
		 * @param right The vector to add to this vector.
		 * @return This vector.
		 */
		VectorBase<T, Size> &operator+=(const VectorBase<T, Size> &right);

		/**
		 * @brief Component-wise subtraction.
		 * @param right The vector to subtract from this vector.
		 * @return This vector.
		 */
		VectorBase<T, Size> &operator-=(const VectorBase<T, Size> &right);

		/**
		 * @brief Component-wise scalar multiplication.
		 * @tparam T2 The scalar's type.
		 * @param right The scalar by which to multiply this vector.
		 * @return This vector.
		 */
		template<typename T2>
		VectorBase<T, Size> &operator*=(const T2 &right);

		/**
		 * @brief Component-wise scalar division.
		 * @tparam T2 The scalar's type.
		 * @param right The scalar by which to divide this vector.
		 * @return This vector.
		 * @throws DivideByZeroException The scalar is zero.
		 */
		template<typename T2>
		VectorBase<T, Size> &operator/=(const T2 &right);

	private:
		/**
		* @brief The vector's components.
		*/
		std::array<T, Size> data;
	};

	/**
	* @brief Returns a normalized copy of a given vector.
	*
	* @tparam T The vector's type.
	* @tparam Size The vector's dimension.
	* @param vector The vector to normalize.
	* @return A normalized copy of the vector.
	*
	* @throws DivideByZeroException The vector's length is zero.
	*/
	template<typename T, size_t Size>
	VectorBase<T, Size> normalize(VectorBase<T, Size> vector);

	/**
	* @brief Calculates the dot product of two vectors.
	*
	* @tparam T The vectors' type.
	* @tparam Size The vectors' dimension.
	* @param left The first vector.
	* @param right The second vector.
	* @return The dot product.
	*/
	template<typename T, size_t Size>
	T dot(const VectorBase<T, Size> &left, const VectorBase<T, Size> &right);

	/**
	 * @brief Computes the angle between two vectors.
	 *
	 * @tparam T The vectors' type.
	 * @tparam Size The vectors' dimension.
	 * @param left The first vector.
	 * @param right The second vector.
	 * @return The angle between the two vectors in radians.
	 *
	 * @throws DivideByZeroException One or both vector's lengths is zero.
	 */
	template<typename T, size_t Size>
	double angle(const VectorBase<T, Size> &left, const VectorBase<T, Size> &right);

	/* Overloaded Operators */
	template<typename T, size_t Size>
	std::ostream &operator<<(
		std::ostream &os,
		const VectorBase<T, Size> &vec);

	template<typename T, size_t Size>
	bool operator==(
		const VectorBase<T, Size> &left,
		const VectorBase<T, Size> &right);

	template<typename T, size_t Size>
	bool operator!=(
		const VectorBase<T, Size> &left,
		const VectorBase<T, Size> &right);

	template<typename T, size_t Size>
	VectorBase<T, Size> operator+(
		const VectorBase<T, Size> &left,
		const VectorBase<T, Size> &right);

	template<typename T, size_t Size>
	VectorBase<T, Size> operator-(
		const VectorBase<T, Size> &left,
		const VectorBase<T, Size> &right);

	template<typename T, size_t Size>
	VectorBase<T, Size> operator*(
		double left,
		const VectorBase<T, Size> &right);

	template<typename T, size_t Size>
	VectorBase<T, Size> operator*(
		const VectorBase<T, Size> &left,
		double right);

	template<typename T, size_t Size>
	VectorBase<T, Size> operator/(
		const VectorBase<T, Size> &left,
		double right);
}

#include "vectorbase.tpp"

#pragma once

#include <array>
#include <initializer_list>
#include <ostream>

#include "../exception.h"

namespace gml
{
	/**
	 * @brief Represents a generic n-dimensional vector.
	 * 
	 * @tparam T the vector's data type
	 * @tparam Size the vector's dimension
	 */
	template <typename T, size_t Size>
	class VectorBase
	{
	private:
		/**
		* @brief The vector fields as an array.
		*/
		std::array<T, Size> data;

	public:
		/**
		 * @brief Initializes all fields with zero.
		 */
		VectorBase();

		/**
		 * @brief Initializes all fieds with the given value.
		 * 
		 * @param value the initializing value
		 */
		explicit VectorBase(T value);

		/**
		* @brief Initializes the fields by an initialzer list.
		*
		* @param init the initializer list
		* 
		* @throws InvalidDimensionException if initializer list is different size
		* from this vector's dimension
		*/
		VectorBase(const std::initializer_list<T>& init);

		VectorBase(const VectorBase<T, Size>& other);

		template <typename T2>
		VectorBase(const VectorBase<T2, Size>& other);

		VectorBase<T, Size>& operator=(const VectorBase<T, Size>& other);

		template <typename T2>
		VectorBase<T, Size>& operator=(const VectorBase<T2, Size>& other);

	public:
		/**
		 * @brief Returns a reference to this vector's field at a given position.
		 * 
		 * @param pos the field's position
		 * @return the reference to the field
		 */
		T& at(size_t pos);

		/**
		 * @brief Returns a const reference to this vector's field at a given 
		 * position.
		 * 
		 * @param pos the field's position
		 * @return the reference to the field
		 */
		const T& at(size_t pos) const;

		/**
		 * @brief Returns the squared length of this vector.
		 * 
		 * @return the squared length
		 */
		double sqrLength() const;

		/**
		 * @brief Returns the length of this vector.
		 * 
		 * @return the length
		 */
		double length() const;

		/**
		 * @brief Normalizes this vector.
		 * 
		 * @return this
		 * @throws DivideByZeroException if this vector's length is zero.
		 */
		VectorBase<T, Size>& normalize();

		/* Overloaded Operators */
		VectorBase<T, Size> operator-() const;
		VectorBase<T, Size>& operator+=(const VectorBase<T, Size>& right);
		VectorBase<T, Size>& operator-=(const VectorBase<T, Size>& right);
		template <typename T2>
		VectorBase<T, Size>& operator*=(const T2& right);
		template <typename T2>
		VectorBase<T, Size>& operator/=(const T2& right);
	};

	/**
	* @brief Returns the normalized vector.
	*
	* @tparam T the type
	* @tparam Size the dimension
	* @param vector the vector to normalize
	* @return the normalized vector
	*
	* @throws DivideByZeroException if the vector's length is zero
	*/
	template <typename T, size_t Size>
	VectorBase<T, Size> normalize(const VectorBase<T, Size>& vector);

	/**
	* @brief Calculates the dot product of two vectors.
	*
	* @tparam T the type
	* @tparam Size the dimension
	* @param left the first vector
	* @param right the second vector
	* @return the dot product
	*/
	template <typename T, size_t Size>
	T dot(const VectorBase<T, Size>& left, const VectorBase<T, Size>& right);

	/* Overloaded Operators */
	template <typename T, size_t Size>
	std::ostream& operator<<(
		std::ostream& os,
		const VectorBase<T, Size>& vec);

	template <typename T, size_t Size>
	bool operator==(
		const VectorBase<T, Size>& left,
		const VectorBase<T, Size>& right);

	template <typename T, size_t Size>
	bool operator!=(
		const VectorBase<T, Size>& left,
		const VectorBase<T, Size>& right);

	template <typename T, size_t Size>
	VectorBase<T, Size> operator+(
		const VectorBase<T, Size>& left,
		const VectorBase<T, Size>& right);

	template <typename T, size_t Size>
	VectorBase<T, Size> operator-(
		const VectorBase<T, Size>& left,
		const VectorBase<T, Size>& right);

	template <typename T, size_t Size, typename T2>
	VectorBase<T, Size> operator*(
		const T2& left,
		const VectorBase<T, Size>& right);

	template <typename T, size_t Size, typename T2>
	VectorBase<T, Size> operator*(
		const VectorBase<T, Size>& left,
		const T2& right);

	template <typename T, size_t Size, typename T2>
	VectorBase<T, Size> operator/(
		const VectorBase<T, Size>& left,
		const T2& right);
}

#include "vectorBase.tpp"

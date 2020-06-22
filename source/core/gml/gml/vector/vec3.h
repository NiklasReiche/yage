#pragma once

#include "vectorBase.h"

namespace gml
{
	/**
	 * @brief Represents a specialized 3-dimensional vector.
	 *
	 * This subclass adds special members x, y, z for convenience.
	 * Vec3<T> and VectorBase<T, 3> can be used interchangeably.
	 *
	 * @tparam T The type of the vector's components.
	 */
	template<typename T>
	class Vec3 : public VectorBase<T, 3>
	{
	public:
		T& x = (*this)(0);
		T& y = (*this)(1);
		T& z = (*this)(2);

		using VectorBase<T, 3>::VectorBase;

		constexpr Vec3(T x, T y, T z)
			: VectorBase<T, 3>({ x, y, z })
		{
		}

		constexpr Vec3(const VectorBase<T, 3>& other)
			: VectorBase<T, 3>(other)
		{
			// Makes Vec3<T> and VectorBase<T, 3> interchangeable
		}

		constexpr Vec3(const Vec3<T>& other)
			: VectorBase<T, 3>(other)
		{
			// We need to define the copy constructor so the x,y,z references do not get copied
		}

		constexpr Vec3<T>& operator=(const Vec3<T>& other)
		{
			// We need to define the copy assignment operator so the x,y,z references do not get copied
			if (this != &other) {
				VectorBase<T, 3>::operator=(other);
			}
			return *this;
		}

		static constexpr Vec3<T> worldForward()
		{
			return Vec3<T>(0, 0, 1);
		}

		static constexpr Vec3<T> worldUp()
		{
			return Vec3<T>(0, 1, 0);
		}

		static constexpr Vec3<T> worldRight()
		{
			return Vec3<T>(-1, 0, 0);
		}
	};

	/**
	* @brief Calculates the cross product of two 3-dimensional vectors.
	*
	* @tparam T the type
	* @param left the first vector
	* @param right the second vector
	* @return the cross product
	*/
	template<typename T>
	constexpr Vec3<T> cross(Vec3<T> left, Vec3<T> right)
	{
		Vec3<T> result;
		result.x = left.y * right.z - left.z * right.y;
		result.y = left.z * right.x - left.x * right.z;
		result.z = left.x * right.y - left.y * right.x;
		return result;
	}
}

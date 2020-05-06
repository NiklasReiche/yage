#pragma once

#include "vectorbase.h"

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
		T & x = this->at(0);
		T & y = this->at(1);
		T & z = this->at(2);

		using VectorBase<T, 3>::VectorBase;

		Vec3(T x, T y, T z);

		Vec3(const Vec3<T> & other);

		Vec3(const VectorBase<T, 3>& other);

		Vec3<T> &operator=(const Vec3<T> &other);

		static Vec3<T> worldForward();

		static Vec3<T> worldUp();

		static Vec3<T> worldRight();
	};

	/**
	* @brief Calculates the cross product of two 3-dimensional vectors.
	*
	* @tparam T the type
	* @param left the first vector
	* @param right the second vector
	* @return the cross product
	*/
	template <typename T>
	Vec3<T> cross(Vec3<T> left, Vec3<T> right);
}

#include "vec3.tpp"

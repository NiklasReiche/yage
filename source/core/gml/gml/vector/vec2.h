#pragma once

#include "vectorbase.h"

namespace gml
{
	/**
	* @brief Represents a 2-dimensional generic vector.
	* This class adds special members x, y for conveniance.
	*
	* @tparam T the type
	*/
	template<typename T>
	class Vec2 : public VectorBase<T, 2>
	{
	public:
		T & x = this->at(0);
		T & y = this->at(1);

		/** Inherit Constructors from VectorBase */
		using VectorBase<T, 2>::VectorBase;

		Vec2();

		Vec2(T x, T y);

		Vec2(const Vec2<T> & other);

		Vec2(const VectorBase<T, 2> & other);

		Vec2<T>& operator=(const Vec2<T> & other);

		Vec2<T>& operator=(const VectorBase<T, 2> & other);
	};

	/**
	* @brief Calculates the angle bewtween two 2-dimensional vectors.
	*
	* @tparam T the type
	* @param left the first vector
	* @param right the second vector
	* @return the angle in radians
	*
	* @throws DivideByZeroException if one or both vector's lengths is zero
	*/
	template <typename T>
	double angle(Vec2<T> left, Vec2<T> right);
}

#include "vec2.tpp"

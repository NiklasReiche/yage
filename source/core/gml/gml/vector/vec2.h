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
}

#include "vec2.tpp"
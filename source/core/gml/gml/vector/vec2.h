#pragma once

#include "vectorbase.h"

namespace gml
{
	/**
	 * @brief Represents a specialized 2-dimensional vector.
	 *
	 * This subclass adds special members x, y for convenience.
	 * Vec2<T> and VectorBase<T, 2> are interchangeably.
	 *
	 * @tparam T The type of the vector's components.
	 */
	template<typename T>
	class Vec2 : public VectorBase<T, 2>
	{
	public:
		T & x = this->at(0);
		T & y = this->at(1);

		using VectorBase<T, 2>::VectorBase;

		Vec2(T x, T y);

		Vec2(const Vec2<T>& other);

		Vec2(const VectorBase<T, 2>& other);

		Vec2<T>& operator=(const Vec2<T>& other);
	};
}

#include "vec2.tpp"

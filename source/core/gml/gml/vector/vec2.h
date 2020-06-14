#pragma once

#include "vectorBase.h"

namespace gml
{
	/**
	 * @brief Represents a specialized 2-dimensional vector.
	 *
	 * This subclass adds special members x, y for convenience.
	 * Vec2<T> and VectorBase<T, 2> can be used interchangeably.
	 *
	 * @tparam T The type of the vector's components.
	 */
	template<typename T>
	class Vec2 : public VectorBase<T, 2>
	{
	public:
		T & x = (*this)(0);
		T & y = (*this)(1);

		using VectorBase<T, 2>::VectorBase;

		Vec2(T x, T y)
			: VectorBase<T, 2>({ x, y })
		{
		}

		Vec2(const Vec2<T>& other)
			: VectorBase<T, 2>(other)
		{
			// We need to define the copy constructor so the x,y references do not get copied
		}

		Vec2(const VectorBase<T, 2>& other)
			: VectorBase<T, 2>(other)
		{
			// Makes Vec2<T> and VectorBase<T, 2> interchangeable
		}

		Vec2<T>& operator=(const Vec2<T>& other)
		{
			// We need to define the copy assignment operator so the x,y references do not get copied
			if (this != &other) {
				VectorBase<T, 2>::operator=(other);
			}
			return *this;
		}
	};
}

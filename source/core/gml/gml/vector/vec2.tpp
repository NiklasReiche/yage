#pragma once

#include "vec2.h"

namespace gml
{
	template<typename T>
	Vec2<T>::Vec2(T x, T y)
		: VectorBase<T, 2>({ x, y })
	{
	}

	// We need to define the copy constructor so the x,y references do not get copied
	template<typename T>
	Vec2<T>::Vec2(const Vec2 <T> &other)
		: VectorBase<T, 2>(other)
	{
	}

	// Makes Vec2<T> and VectorBase<T, 2> interchangeable
	template<typename T>
	Vec2<T>::Vec2(const VectorBase<T, 2> &other)
		: VectorBase<T, 2>(other)
	{
	}


	// We need to define the copy assignment operator so the x,y references do not get copied
	template<typename T>
	Vec2 <T> &Vec2<T>::operator=(const Vec2 <T> &other)
	{
		if (this != &other)
		{
			VectorBase<T, 2>::operator=(other);
		}
		return *this;
	}
}

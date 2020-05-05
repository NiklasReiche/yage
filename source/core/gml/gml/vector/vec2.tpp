#pragma once

#include "vec2.h"

namespace gml
{
	template <typename T>
	Vec2<T>::Vec2()
		: VectorBase<T, 2>()
	{
	}

	template <typename T>
	Vec2<T>::Vec2(T x, T y)
		: VectorBase<T, 2>({ x, y })
	{
	}

	template <typename T>
	Vec2<T>::Vec2(const Vec2<T>& other)
		: VectorBase<T, 2>(other)
	{
	}

	template <typename T>
	Vec2<T>::Vec2(const VectorBase<T, 2>& other)
		: VectorBase<T, 2>(other)
	{
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator=(const Vec2<T>& other)
	{
		VectorBase<T, 2>::operator=(other);
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator=(const VectorBase<T, 2>& other)
	{
		VectorBase<T, 2>::operator=(other);
		return *this;
	}
}

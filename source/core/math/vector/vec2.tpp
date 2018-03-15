#pragma once

namespace gml
{
	template <typename T>
	Vec2<T>::Vec2()
		: VectorBase()
	{
	}

	template <typename T>
	Vec2<T>::Vec2(T x, T y)
		: VectorBase({ x, y })
	{
	}

	template <typename T>
	Vec2<T>::Vec2(const Vec2<T>& other)
		: VectorBase(other)
	{
	}

	template <typename T>
	Vec2<T>::Vec2(const VectorBase<T, 2>& other)
		: VectorBase(other)
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

	template <typename T>
	float angle(Vec2<T> left, Vec2<T> right)
	{
		return std::acos(dot(normalize(left), normalize(right)));
	}
}

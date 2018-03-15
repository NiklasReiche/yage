#pragma once

namespace gml
{
	template <typename T>
	Vec4<T>::Vec4()
		: VectorBase<T, 4>()
	{
	}

	template <typename T>
	Vec4<T>::Vec4(T x, T y, T z, T w)
		: VectorBase({ x, y, z, w })
	{
	}

	template <typename T>
	Vec4<T>::Vec4(const Vec4<T>& other)
		: VectorBase(other)
	{
	}

	template <typename T>
	Vec4<T>::Vec4(const VectorBase<T, 4>& other)
		: VectorBase(other)
	{
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator=(const Vec4<T>& other)
	{
		VectorBase<T, 3>::operator=(other);
		return *this;
	}

	template <typename T>
	Vec4<T>& Vec4<T>::operator=(const VectorBase<T, 4>& other)
	{
		VectorBase<T, 4>::operator=(other);
		return *this;
	}
}

#pragma once

namespace gml
{
	template <typename T>
	Vec4<T>::Vec4(T x, T y, T z, T w)
		: VectorBase<T, 4>({ x, y, z, w })
	{
	}

	// We need to define the copy constructor so the x,y,z,w references do not get copied
	template <typename T>
	Vec4<T>::Vec4(const Vec4<T>& other)
		: VectorBase<T, 4>(other)
	{
	}

	// Makes Vec4<T> and VectorBase<T, 4> interchangeable
	template <typename T>
	Vec4<T>::Vec4(const VectorBase<T, 4>& other)
		: VectorBase<T, 4>(other)
	{
	}

	// We need to define the copy assignment operator so the x,y,z,w references do not get copied
	template <typename T>
	Vec4<T>& Vec4<T>::operator=(const Vec4<T>& other)
	{
		if (this != &other)
		{
			VectorBase<T, 4>::operator=(other);
		}
		return *this;
	}
}

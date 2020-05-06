#pragma once

#include "vec3.h"

namespace gml
{
	template <typename T>
	Vec3<T>::Vec3(T x, T y, T z)
		: VectorBase<T, 3>({ x, y, z })
	{
	}

	// We need to define the copy constructor so the x,y,z references do not get copied
	template <typename T>
	Vec3<T>::Vec3(const Vec3<T>& other)
		: VectorBase<T, 3>(other)
	{
	}

	// Makes Vec3<T> and VectorBase<T, 3> interchangeable
	template<typename T>
	Vec3<T>::Vec3(const VectorBase<T, 3> &other)
		: VectorBase<T, 3>(other)
	{
	}

	// We need to define the copy assignment operator so the x,y,z references do not get copied
	template <typename T>
	Vec3<T>& Vec3<T>::operator=(const Vec3<T>& other)
	{
		if (this != &other)
		{
			VectorBase<T, 3>::operator=(other);
		}
		return *this;
	}

	template <typename T>
	Vec3<T> Vec3<T>::worldForward()
	{
		return Vec3<T>(0, 0, 1);
	}

	template <typename T>
	Vec3<T> Vec3<T>::worldUp()
	{
		return Vec3<T>(0, 1, 0);
	}

	template <typename T>
	Vec3<T> Vec3<T>::worldRight()
	{
		return Vec3<T>(-1, 0, 0);
	}

	template <typename T>
	Vec3<T> cross(Vec3<T> left, Vec3<T> right)
	{
		Vec3<T> result;
		result.x = left.y * right.z - left.z * right.y;
		result.y = left.z * right.x - left.x * right.z;
		result.z = left.x * right.y - left.y * right.x;
		return result;
	}
}

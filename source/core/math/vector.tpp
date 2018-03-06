#pragma once

namespace gml
{
	template <typename T>
	Vec1<T>::Vec1()
		: VectorBase<T, 1>()
	{
	}

	template <typename T>
	Vec1<T>::Vec1(T x)
		: VectorBase({x})
	{
	}

	template <typename T>
	Vec1<T>::Vec1(const Vec1<T>& other)
		: VectorBase(other)
	{
	}

	template <typename T>
	Vec1<T>::Vec1(const VectorBase<T, 1>& other)
		: VectorBase(other)
	{
	}

	template <typename T>
	Vec1<T>& Vec1<T>::operator=(const Vec1<T>& other)
	{
		VectorBase<T, 1>::operator=(other);
		return *this;
	}

	template <typename T>
	Vec1<T>& Vec1<T>::operator=(const VectorBase<T, 1>& other)
	{
		VectorBase<T, 1>::operator=(other);
		return *this;
	}

	template <typename T>
	Vec2<T>::Vec2()
		: VectorBase()
	{
	}

	template <typename T>
	Vec2<T>::Vec2(T x, T y)
		: VectorBase({x, y})
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
	Vec3<T>::Vec3()
		: VectorBase<T, 3>()
	{
	}

	template <typename T>
	Vec3<T>::Vec3(T x, T y, T z)
		: VectorBase({x, y, z})
	{
	}

	template <typename T>
	Vec3<T>::Vec3(const Vec3<T>& other)
		: VectorBase(other)
	{
	}

	template <typename T>
	Vec3<T>::Vec3(const VectorBase<T, 3>& other)
		: VectorBase(other)
	{
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator=(const Vec3<T>& other)
	{
		VectorBase<T, 3>::operator=(other);
		return *this;
	}

	template <typename T>
	Vec3<T>& Vec3<T>::operator=(const VectorBase<T, 3>& other)
	{
		VectorBase<T, 3>::operator=(other);
		return *this;
	}

	template <typename T>
	Vec4<T>::Vec4()
		: VectorBase<T, 4>()
	{
	}

	template <typename T>
	Vec4<T>::Vec4(T x, T y, T z, T w)
		: VectorBase({x, y, z, w})
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

	template <typename T>
	Vec3<T> cross(Vec3<T> left, Vec3<T> right)
	{
		Vec3<T> result;
		result.x = left.y * right.z - left.z * right.y;
		result.y = left.z * right.x - left.x * right.z;
		result.z = left.x * right.y - left.y * right.x;
		return result;
	}

	template <typename T>
	float angle(Vec2<T> left, Vec2<T> right)
	{
		return std::acos(dot(normalize(left), normalize(right)));
	}

	template <typename T>
	float angle(Vec3<T> left, Vec3<T> right)
	{
		const double lengths = left.length() * right.length();
		if (lengths == 0.0)
		{
			throw DivideByZeroException();
		}
		return std::acos(dot(left, right) / lengths);
	}
}

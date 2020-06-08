#pragma once

#include "vectorbase.h"

namespace gml
{
	/**
	 * @brief Represents a specialized 3-dimensional vector.
	 *
	 * This subclass adds special members x, y, z, w for convenience.
	 * Vec4<T> and VectorBase<T, 4> can be used interchangeably.
	 *
	 * @tparam T The type of the vector's components.
	 */
	template<typename T>
	class Vec4 : public VectorBase<T, 4>
	{
	public:
		T& x = (*this)(0);
		T& y = (*this)(1);
		T& z = (*this)(2);
		T& w = (*this)(3);

		using VectorBase<T, 4>::VectorBase;

		Vec4(T x, T y, T z, T w)
			: VectorBase<T, 4>({ x, y, z, w })
		{
		}

		Vec4(const Vec4<T>& other)
			: VectorBase<T, 4>(other)
		{
			// We need to define the copy constructor so the x,y,z,w references do not get copied
		}

		Vec4(const VectorBase<T, 4>& other)
			: VectorBase<T, 4>(other)
		{
			// Makes Vec4<T> and VectorBase<T, 4> interchangeable
		}

		Vec4<T>& operator=(const Vec4<T>& other)
		{
			if (this != &other) {
				VectorBase<T, 4>::operator=(other);
			}
			return *this;
		}
	};
}

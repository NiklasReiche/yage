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
		T & x = this->at(0);
		T & y = this->at(1);
		T & z = this->at(2);
		T & w = this->at(3);

		using VectorBase<T, 4>::VectorBase;

		Vec4(T x, T y, T z, T w);

		Vec4(const Vec4<T> & other);

		Vec4(const VectorBase<T, 4> & other);

		Vec4<T>& operator=(const Vec4<T> & other);
	};
}

#include "vec4.tpp"

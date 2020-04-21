#pragma once

#include "vectorbase.h"

namespace gml
{
	/**
	* @brief Represents a 4-dimensional generic vector.
	* This class adds special members x, y, z, w for conveniance.
	*
	* @tparam T the type
	*/
	template<typename T>
	class Vec4 : public VectorBase<T, 4>
	{
	public:
		T & x = this->at(0);
		T & y = this->at(1);
		T & z = this->at(2);
		T & w = this->at(3);

		/** Inherit Constructors from VectorBase */
		using VectorBase<T, 4>::VectorBase;

		Vec4();

		Vec4(T x, T y, T z, T w);

		Vec4(const Vec4<T> & other);

		Vec4(const VectorBase<T, 4> & other);

		Vec4<T>& operator=(const Vec4<T> & other);

		Vec4<T>& operator=(const VectorBase<T, 4> & other);
	};
}

#include "vec4.tpp"

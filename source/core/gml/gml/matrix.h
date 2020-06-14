#pragma once

#include "matrix/matrixBase.h"
#include "matrix/matrixOperations.h"
#include "matrix/matrixConstructs.h"
#include "matrix/mat2.h"
#include "matrix/mat3.h"
#include "matrix/mat4.h"

namespace gml
{
	template<typename T, std::size_t Rows, std::size_t Columns>
	using Matrix = MatrixBase<T, Rows, Columns>;

	using Mat2i = Mat2<int>;
	using Mat2u = Mat2<unsigned int>;
	using Mat2f = Mat2<float>;
	using Mat2d = Mat2<double>;

	using Mat3i = Mat3<int>;
	using Mat3u = Mat3<unsigned int>;
	using Mat3f = Mat3<float>;
	using Mat3d = Mat3<double>;

	using Mat4i = Mat4<int>;
	using Mat4u = Mat4<unsigned int>;
	using Mat4f = Mat4<float>;
	using Mat4d = Mat4<double>;
}

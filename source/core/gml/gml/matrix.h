#pragma once

#include "matrix/matrixBase.h"
#include "matrix/matrixOperations.h"
#include "matrix/mat2.h"
#include "matrix/mat3.h"
#include "matrix/mat4.h"

namespace gml
{
	template <typename T, std::size_t Rows, std::size_t Columns>
	using Matrix = MatrixBase<T, Rows, Columns>;

	typedef Mat2<int> Mat2i;
	typedef Mat2<unsigned int> Mat2ui;
	typedef Mat2<float> Mat2f;
	typedef Mat2<double> Mat2d;

	typedef Mat3<int> Mat3i;
	typedef Mat3<unsigned int> Mat3ui;
	typedef Mat3<float> Mat3f;
	typedef Mat3<double> Mat3d;

	typedef Mat4<int> Mat4i;
	typedef Mat4<unsigned int> Mat4ui;
	typedef Mat4<float> Mat4f;
	typedef Mat4<double> Mat4d;
}

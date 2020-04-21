#pragma once

#include "vector/vectorbase.h"
#include "vector/vec2.h"
#include "vector/vec3.h"
#include "vector/vec4.h"

namespace gml
{
	template <typename T, size_t Size>
	using Vector = VectorBase<T, Size>;

	typedef Vec2<int> Vec2i;
	typedef Vec2<unsigned int> Vec2ui;
	typedef Vec2<float> Vec2f;
	typedef Vec2<double> Vec2d;

	typedef Vec3<int> Vec3i;
	typedef Vec3<unsigned int> Vec3ui;
	typedef Vec3<float> Vec3f;
	typedef Vec3<double> Vec3d;

	typedef Vec4<int> Vec4i;
	typedef Vec4<unsigned int> Vec4ui;
	typedef Vec4<float> Vec4f;
	typedef Vec4<double> Vec4d;
}

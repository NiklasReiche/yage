#pragma once

#include "vector/vectorBase.h"
#include "vector/vectorOperations.h"
#include "vector/vec2.h"
#include "vector/vec3.h"
#include "vector/vec4.h"

namespace gml
{
	template<typename T, std::size_t Size>
	using Vector = VectorBase<T, Size>;

	using Vec2i = Vec2<int>;
	using Vec2u = Vec2<unsigned int>;
	using Vec2f = Vec2<float>;
	using Vec2d = Vec2<double>;

	using Vec3i = Vec3<int>;
	using Vec3u = Vec3<unsigned int>;
	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;

	using Vec4i = Vec4<int>;
	using Vec4u = Vec4<unsigned int>;
	using Vec4f = Vec4<float>;
	using Vec4d = Vec4<double>;
}

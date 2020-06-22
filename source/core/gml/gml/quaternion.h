#pragma once

#include "quaternion/quaternion.h"
#include "quaternion/quaternionOperations.h"
#include "quaternion/quaternionConstructs.h"

namespace gml
{
	template<typename T>
	using Quat = Quaternion<T>;

	using Quati = Quaternion<int>;
	using Quatu = Quaternion<unsigned int>;
	using Quatf = Quaternion<float>;
	using Quatd = Quaternion<double>;
}

#pragma once

#include <cmath>

namespace gml
{
	const float PI = 3.141592653589793238463f;

	inline double toRad(const double deg)
	{
		return deg * (PI / 180.0);
	}
	inline double toDeg(const double rad)
	{
		return rad * (180.0 / PI);
	}
}
#pragma once

namespace gml
{
	const float PI = 3.141592653589793238463f;

	inline float toRad(float deg)
	{
		return deg * (PI / 180);
	}
	inline float toDeg(float rad)
	{
		return rad * (180 / PI);
	}
}
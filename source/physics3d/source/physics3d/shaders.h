#pragma once

#include <string>

namespace physics3d::shaders
{
	struct PointShader
	{
		static const std::string vert;
        static const std::string geom;
        static const std::string frag;
	};
}
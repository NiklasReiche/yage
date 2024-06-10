#pragma once

#include <string>

namespace yage::physics3d::shaders
{
	struct PointShader
	{
		static const std::string vert;
        static const std::string geom;
        static const std::string frag;
	};

    struct VectorShader
    {
        static const std::string vert;
        static const std::string geom;
        static const std::string frag;
    };
}
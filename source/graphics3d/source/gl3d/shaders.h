#pragma once

#include <string>

namespace gl3d::shaders
{
	struct PbrNormalMappingShader
	{
		static const std::string vert;
        static const std::string frag;
	};

    struct PbrShader
    {
        static const std::string vert;
        static const std::string frag;
    };
}
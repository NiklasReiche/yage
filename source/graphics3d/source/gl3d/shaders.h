#pragma once

#include <string>

namespace yage::gl3d::shaders
{
	struct PbrNormalMappingShader
	{
		static const std::string vert;
        static const std::string frag;

		static std::string get_vert();

		static std::string get_frag();
	};

    struct PbrShader
    {
        static const std::string vert;
        static const std::string frag;

    	static std::string get_vert();

        static std::string get_frag();
    };
}
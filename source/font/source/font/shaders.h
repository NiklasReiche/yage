#pragma once

#include <string>

namespace font 
{
	struct ShaderTemplate
	{
#ifdef WIN32 //TODO
		const std::string textShader_vert =
#include "shaders/textShader_GL3.vert"
			;
		const std::string textShader_frag =
#include "shaders/textShader_GL3.frag"
        ;
#endif // WIN32

#ifdef ANDROID
		const std::string textShader_vert =
#include "shaders/textShader_ES1.vert"
			;
		const std::string textShader_frag =
#include "shaders/textShader_ES1.frag"
			;
#endif // ANDROID
	};
}
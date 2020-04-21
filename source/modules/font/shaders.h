#pragma once

#include <string>

namespace font 
{
	struct ShaderTemplate
	{
#ifdef DESKTOP
		const std::string textShader_vert =
#include "shaders/textShader_GL3.vert"
			;
		const std::string textShader_frag =
#include "shaders/textShader_GL3.frag"
			;
#endif // DESKTOP 

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
#pragma once

#include <string>

namespace gui
{
	struct ShaderTemplate 
	{
#ifdef DESKTOP
		const std::string guiShader_vert =
#include "shaders/guiShader_GL3.vert"
			;
		const std::string guiShader_frag =
#include "shaders/guiShader_GL3.frag"
			;
#endif

#ifdef ANDROID
		const std::string guiShader_vert =
#include "shaders/guiShader_ES1.vert"
			;
		const std::string guiShader_frag =
#include "shaders/guiShader_ES1.frag"
			;
#endif
	};
}
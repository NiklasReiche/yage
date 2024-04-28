#pragma once

#include <string>

namespace gui
{
	struct ShaderTemplate 
	{
		const std::string guiShader_vert =
#include "shaders/guiShader_GL3.vert"
			;
		const std::string guiShader_frag =
#include "shaders/guiShader_GL3.frag"
			;
	};
}
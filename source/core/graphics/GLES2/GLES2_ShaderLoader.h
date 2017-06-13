#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <platform\Platform.h>

#include "gles2.h"
#include "GLES2_Context.h"
#include "GLES2_Shader.h"

namespace gles2
{
	class GLES2_ShaderLoader
	{
	private:
		platform::PlatformHandle* systemHandle;
		GLES2_Context* glContext;

	public:
		GLES2_ShaderLoader(platform::PlatformHandle* systemHandle, GLES2_Context* glContext);

		GLES2_Shader loadShader(std::string vertex_path, std::string fragment_path);
	};
}
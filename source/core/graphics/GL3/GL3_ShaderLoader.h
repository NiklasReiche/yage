#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "gl3.h"
#include "GL3_Context.h"
#include "GL3_Shader.h"

namespace gl3
{
	class GL3_ShaderLoader
	{
	private:
		sys::PlatformHandle* systemHandle;
		GL3_Context* glContext;

	public:
		GL3_ShaderLoader(sys::PlatformHandle* systemHandle, GL3_Context* glContext);

		GL3_Shader loadFromFile(std::string vertex_path, std::string fragment_path, std::string geometry_path = "");
		GL3_Shader loadFromString(std::string vertex_string, std::string fragment_string, std::string geometry_string = "");
	};
}
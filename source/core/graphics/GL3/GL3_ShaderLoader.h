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
		GL3_Context* glContext;

	public:
		GL3_ShaderLoader(GL3_Context* glContext);

		GL3_Shader loadShader(std::string vertex_path, std::string fragment_path, std::string geometry_path = "");
	};
}
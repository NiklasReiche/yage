#pragma once

#include <string>
#include <map>

#include "math\MVector.h"

#include "gl3.h"
#include "GL3_Object.h"

namespace gl3
{
	class GL3_Shader : public GL3_Object
	{
	private:
		friend class GL3_Context;

		GLuint program = 0;
		std::map<std::string, GLint> uniformLocations;

	public:
		GL3_Shader();
		GL3_Shader(const GL3_Shader& other);
		~GL3_Shader();
		GL3_Shader& operator=(const GL3_Shader& other);

		void setUniform(std::string name, int value);
		void setUniform(std::string name, bool value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, gml::Vec3<float> value);
		void setUniform(std::string name, gml::Matrix4D<float> value);
	};
}
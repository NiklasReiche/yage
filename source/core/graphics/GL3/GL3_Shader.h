#pragma once

#include <string>
#include <map>

#include "gl3.h"
#include "GL3_Object.h"

#include "math\MVector.h"

namespace gl3
{
	class GL3_Shader : public GL3_Object
	{
	private:
		friend class GL3_Context;

		std::map<std::string, GLint> uniformLocations;

	public:
		void setUniform(std::string name, int value);
		void setUniform(std::string name, bool value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, gml::Vec3<float> value);
		void setUniform(std::string name, gml::Matrix4D<float> value);
	};
}
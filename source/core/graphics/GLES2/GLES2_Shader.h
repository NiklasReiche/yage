#pragma once

#include <map>

#include "math\MVector.h"

#include "gles2.h"
#include "GLES2_Object.h"

namespace gles2
{
	class GLES2_Shader : public GLES2_Object
	{
	private:
		friend class GLES2_Context;

		std::map<std::string, GLint> uniformLocations;

	public:
		void setUniform(std::string name, int value);
		void setUniform(std::string name, bool value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, gml::Vec3<float> value);
		void setUniform(std::string name, gml::Matrix4D<float> value);
	};
}
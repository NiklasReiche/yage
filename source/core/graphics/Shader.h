#pragma once

#include <map>

#include "Platform.h"
#include "GType.h"

#include "math\MVector.h"

namespace gl
{
	struct Shader
	{
		Guint id;
		std::map<std::string, Gint> uniformLocations;

		void setUniform(std::string name, int value);
		void setUniform(std::string name, bool value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, gml::Vec3<float> value);
		void setUniform(std::string name, gml::Matrix4D<float> value);
	};
}
#pragma once

#include <map>

#include <math/vector.h>
#include <math/matrix.h>

#include "gles2.h"
#include "GLES2_Object.h"

namespace gles2
{
	class GLES2_Shader : public GLES2_Object
	{
	private:
		friend class GLES2_Context;

		GLuint program = 0;
		std::map<std::string, GLint> uniformLocations;

	public:
		GLES2_Shader();
		GLES2_Shader(const GLES2_Shader& other);
		~GLES2_Shader();
		GLES2_Shader& operator=(const GLES2_Shader& other);

		void setUniform(std::string name, int value);
		void setUniform(std::string name, bool value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, math::Vec3<float> value);
		void setUniform(std::string name, math::Matrix4D<float> value);
	};
}

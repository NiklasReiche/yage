#pragma once

#include <string>
#include <map>

#include "gl3.h"
#include "GL3_Object.h"
#include "math/vector.h"
#include "math/matrix.h"

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
		virtual ~GL3_Shader();
		GL3_Shader& operator=(const GL3_Shader& other);

		/**
		 * @brief Checks whether the shader has the given uniform.
		 * 
		 * @param name the uniform's name
		 * @return true when the shader has the uniform
		 * @return false when th shader does not have the uniform
		 */
		bool hasUniform(const std::string& name) const;

		void setUniform(std::string name, int value);
		void setUniform(std::string name, bool value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, gml::Vec3f value);
		void setUniform(std::string name, gml::Mat4f value);
	};
}

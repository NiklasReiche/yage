#include "GL3_Shader.h"
#include "GL3_Context.h"

namespace gl3
{
	void GL3_Shader::setUniform(std::string name, int value)
	{
		glContext->useShader(*this);
		glUniform1i(uniformLocations[name], value);
	}
	void GL3_Shader::setUniform(std::string name, bool value)
	{
		glContext->useShader(*this);
		glUniform1i(uniformLocations[name], (int)value);
	}
	void GL3_Shader::setUniform(std::string name, float value)
	{
		glContext->useShader(*this);
		glUniform1f(uniformLocations[name], value);
	}
	void GL3_Shader::setUniform(std::string name, gml::Vec3<float> value)
	{
		glContext->useShader(*this);
		glUniform3f(uniformLocations[name], value.x, value.y, value.z);
	}
	void GL3_Shader::setUniform(std::string name, gml::Matrix4D<float> value)
	{
		float matrix[16];
		value.convertToArray(matrix);
		glContext->useShader(*this);
		glUniformMatrix4fv(uniformLocations[name], 1, GL_TRUE, matrix);
	}
}
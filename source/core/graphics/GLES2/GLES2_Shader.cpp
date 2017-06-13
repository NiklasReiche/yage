#include "GLES2_Shader.h"
#include "GLES2_Context.h"

namespace gles2
{
	void GLES2_Shader::setUniform(std::string name, int value)
	{
		glContext->useShader(*this);
		glUniform1i(uniformLocations[name], value);
	}
	void GLES2_Shader::setUniform(std::string name, bool value)
	{
		glContext->useShader(*this);
		glUniform1i(uniformLocations[name], (int)value);
	}
	void GLES2_Shader::setUniform(std::string name, float value)
	{
		glContext->useShader(*this);
		glUniform1f(uniformLocations[name], value);
	}
	void GLES2_Shader::setUniform(std::string name, gml::Vec3<float> value)
	{
		glContext->useShader(*this);
		glUniform3f(uniformLocations[name], value.x, value.y, value.z);
	}
	void GLES2_Shader::setUniform(std::string name, gml::Matrix4D<float> value)
	{
		float matrix[16];
		value.convertToArray(matrix);
		glContext->useShader(*this);
		glUniformMatrix4fv(uniformLocations[name], 1, GL_TRUE, matrix);
	}
}
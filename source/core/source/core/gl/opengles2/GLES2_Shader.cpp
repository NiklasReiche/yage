#include "GLES2_Shader.h"
#include "GLES2_Context.h"

namespace gles2
{
	GLES2_Shader::GLES2_Shader()
		: GLES2_Object() {}
	GLES2_Shader::GLES2_Shader(const GLES2_Shader& other)
		: GLES2_Object(other)
	{
		this->program = other.program;
		this->uniformLocations = other.uniformLocations;
	}
	GLES2_Shader::~GLES2_Shader()
	{
		if (*refCount == 1) {
			glDeleteProgram(program);
		}
	}
	GLES2_Shader& GLES2_Shader::operator=(const GLES2_Shader& other)
	{
		GLES2_Object::operator=(other);
		if (shouldDelete) {
			glDeleteProgram(program);
			shouldDelete = false;
		}

		this->program = other.program;
		this->uniformLocations = other.uniformLocations;

		return *this;
	}

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
	void GLES2_Shader::setUniform(std::string name, math::Vec3<float> value)
	{
		glContext->useShader(*this);
		glUniform3f(uniformLocations[name], value.x, value.y, value.z);
	}
	void GLES2_Shader::setUniform(std::string name, math::Matrix4D<float> value)
	{
        value = transpose(value);
		float matrix[16];
		value.convertToArray(matrix);
		glContext->useShader(*this);
		glUniformMatrix4fv(uniformLocations[name], 1, GL_FALSE, matrix);
	}
}
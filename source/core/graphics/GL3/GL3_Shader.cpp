#include "GL3_Shader.h"
#include "GL3_Context.h"

namespace gl3
{
	GL3_Shader::GL3_Shader()
		: GL3_Object() {}
	GL3_Shader::GL3_Shader(const GL3_Shader& other)
		: GL3_Object(other)
	{
		this->program = other.program;
		this->uniformLocations = other.uniformLocations;
	}
	GL3_Shader::~GL3_Shader()
	{
		if (*refCount == 1) {
			glDeleteProgram(program);
		}
	}
	GL3_Shader& GL3_Shader::operator=(const GL3_Shader& other)
	{
		GL3_Object::operator=(other);
		if (shouldDelete) {
			glDeleteProgram(program);
			shouldDelete = false;
		}

		this->program = other.program;
		this->uniformLocations = other.uniformLocations;

		return *this;
	}

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
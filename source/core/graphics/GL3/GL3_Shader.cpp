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

	bool GL3_Shader::hasUniform(const std::string & name) const
	{
		return uniformLocations.count(name) > 0;
	}

	void GL3_Shader::setUniform(std::string name, int value)
	{
		if (!hasUniform(name)) return;
		glContext->useShader(*this);
		glUniform1i(uniformLocations.at(name), value);
	}
	void GL3_Shader::setUniform(std::string name, bool value)
	{
		if (!hasUniform(name)) return;
		glContext->useShader(*this);
		glUniform1i(uniformLocations.at(name), (int)value);
	}
	void GL3_Shader::setUniform(std::string name, float value)
	{
		if (!hasUniform(name)) return;
		glContext->useShader(*this);
		glUniform1f(uniformLocations.at(name), value);
	}
	void GL3_Shader::setUniform(std::string name, gml::Vec3f value)
	{
		if (!hasUniform(name)) return;
		glContext->useShader(*this);
		glUniform3f(uniformLocations.at(name), value.x, value.y, value.z);
	}
	void GL3_Shader::setUniform(std::string name, gml::Mat4f value)
	{
		if (!hasUniform(name)) return;
		float matrix[16];
		value.convertToArray(matrix);
		glContext->useShader(*this);
		glUniformMatrix4fv(uniformLocations.at(name), 1, GL_TRUE, matrix);
	}
}

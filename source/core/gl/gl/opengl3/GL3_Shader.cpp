#include "GL3_Shader.h"
#include "GL3_Context.h"

namespace gl3
{
	GL3_Shader::~GL3_Shader()
	{
		glDeleteProgram(program);
	}

	GL3_Shader::GL3_Shader(GL3_Shader&& other) noexcept
		: GL3_Object(std::move(other))
	{
		this->program = other.program;
		this->uniformLocations = other.uniformLocations;

		other.program = 0;
		other.uniformLocations = std::map<std::string, GLint>();
	}	
	
	GL3_Shader& GL3_Shader::operator=(GL3_Shader&& other) noexcept
	{
		if (this != &other)
		{
			GL3_Object::operator=(std::move(other));

			this->program = other.program;
			this->uniformLocations = other.uniformLocations;

			other.program = 0;
			other.uniformLocations = std::map<std::string, GLint>();
		}

		return *this;
	}

	bool GL3_Shader::hasUniform(const std::string & name) const
	{
		return uniformLocations.count(name) > 0;
	}

	void GL3_Shader::setUniform(const std::string& name, const int value)
	{
		if (!hasUniform(name)) 
			return;
		
		lockContextPtr()->bindShader(program);
		glUniform1i(uniformLocations.at(name), value);
	}
	
	void GL3_Shader::setUniform(const std::string& name, const bool value)
	{
		if (!hasUniform(name)) 
			return;
		
		lockContextPtr()->bindShader(program);
		glUniform1i(uniformLocations.at(name), static_cast<int>(value));
	}
	
	void GL3_Shader::setUniform(const std::string& name, const float value)
	{
		if (!hasUniform(name)) 
			return;
		
		lockContextPtr()->bindShader(program);
		glUniform1f(uniformLocations.at(name), value);
	}
	
	void GL3_Shader::setUniform(const std::string& name, const gml::Vec3f value)
	{
		if (!hasUniform(name)) 
			return;
		
		lockContextPtr()->bindShader(program);
		glUniform3f(uniformLocations.at(name), value.x, value.y, value.z);
	}
	
	void GL3_Shader::setUniform(const std::string& name, const gml::Mat4f value)
	{
		if (!hasUniform(name)) 
			return;
		
		lockContextPtr()->bindShader(program);
		glUniformMatrix4fv(uniformLocations.at(name), 1, GL_TRUE, value.data());
	}
}

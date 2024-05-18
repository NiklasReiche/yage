#include <iostream>
#include "Shader.h"
#include "Context.h"
#include "UniformBuffer.h"

namespace opengl
{
	Shader::~Shader()
	{
		glDeleteProgram(program);
        lockContextPtr()->bindShader(0);
	}

	Shader::Shader(Shader&& other) noexcept
		: OpenGlObject(std::move(other))
	{
		this->program = other.program;
		this->uniformLocations = other.uniformLocations;

		other.program = 0;
		other.uniformLocations = std::map<std::string, GLint>();
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this != &other)
		{
			OpenGlObject::operator=(std::move(other));

			this->program = other.program;
			this->uniformLocations = other.uniformLocations;

			other.program = 0;
			other.uniformLocations = std::map<std::string, GLint>();
		}

		return *this;
	}

	bool Shader::hasUniform(const std::string & name) const
	{
		return uniformLocations.count(name) > 0;
	}

	void Shader::setUniform(const std::string& name, const int value)
	{
		if (!hasUniform(name))
            throw std::invalid_argument("unknown uniform '" + name + "'");
		
		lockContextPtr()->bindShader(program);
		glUniform1i(uniformLocations.at(name), value);
	}
	
	void Shader::setUniform(const std::string& name, const bool value)
	{
		if (!hasUniform(name))
            throw std::invalid_argument("unknown uniform '" + name + "'");
		
		lockContextPtr()->bindShader(program);
		glUniform1i(uniformLocations.at(name), static_cast<int>(value));
	}
	
	void Shader::setUniform(const std::string& name, const float value)
	{
		if (!hasUniform(name))
            throw std::invalid_argument("unknown uniform '" + name + "'");
		
		lockContextPtr()->bindShader(program);
		glUniform1f(uniformLocations.at(name), value);
	}
	
	void Shader::setUniform(const std::string& name, const gml::Vec3f value)
	{
		if (!hasUniform(name))
            throw std::invalid_argument("unknown uniform '" + name + "'");
		
		lockContextPtr()->bindShader(program);
		glUniform3f(uniformLocations.at(name), value.x(), value.y(), value.z());
	}
	
	void Shader::setUniform(const std::string& name, const gml::Mat4f value)
	{
		if (!hasUniform(name)) 
			throw std::invalid_argument("unknown uniform '" + name + "'");
		
		lockContextPtr()->bindShader(program);
		glUniformMatrix4fv(uniformLocations.at(name), 1, GL_TRUE, value.data());
	}

	void Shader::linkUniformBlock(const gl::IUniformBlock& uniformBlock)
	{
		auto& ubo = static_cast<const UniformBuffer&>(uniformBlock);
		auto context = lockContextPtr();

		int bindPoint = context->getUboBindPoint(ubo.getId());
		int blockIndex = glGetUniformBlockIndex(program, ubo.getName().c_str());
		glUniformBlockBinding(program, blockIndex, bindPoint);
	}
}

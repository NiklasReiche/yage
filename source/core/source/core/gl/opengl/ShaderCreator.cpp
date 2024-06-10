#include <iostream>
#include "ShaderCreator.h"

#include "Context.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace yage::opengl
{
	void ShaderCreator::checkShaderCompilationError(const GLuint program, const ShaderType type)
	{
		int success;
		char infoLog[512];

		if (type == ShaderType::SHADER_PROGRAM)
		{
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(program, 512, nullptr, infoLog);
				throw ShaderCompilationException(type, infoLog);
			}
		}
		else
		{
			glGetShaderiv(program, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(program, 512, nullptr, infoLog);
				throw ShaderCompilationException(type, infoLog);
			}
		}
	}

	std::unique_ptr<gl::IShader> ShaderCreator::createShader(
		const std::string& vertexCode,
		const std::string& fragmentCode,
		const std::string& geometryCode)
	{
		auto shader = std::unique_ptr<Shader>(new Shader(lockContextPtr()));

		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();
		const char* geometrySource = geometryCode.c_str();
		
		// compile vertex shader
		const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		checkShaderCompilationError(vertexShader, ShaderType::VERTEX_SHADER);
		
		// compile fragment shader
		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		checkShaderCompilationError(fragmentShader, ShaderType::FRAGMENT_SHADER);
		
		// compile geometry shader
		GLuint geometryShader = 0;
		if (!geometryCode.empty()) {
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &geometrySource, nullptr);
			glCompileShader(geometryShader);
			checkShaderCompilationError(geometryShader, ShaderType::GEOMETRY_SHADER);
		}
		
		// link shader program
		shader->id = glCreateProgram();
		glAttachShader(shader->id, vertexShader);
		glAttachShader(shader->id, fragmentShader);
		if (!geometryCode.empty()) {
			glAttachShader(shader->id, geometryShader);
		}
		glLinkProgram(shader->id);
		checkShaderCompilationError(shader->id, ShaderType::SHADER_PROGRAM);
		
		// delete shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (!geometryCode.empty()) {
			glDeleteShader(geometryShader);
		}

		shader->program = shader->id;

		// Save uniform locations
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		GLsizei bufSize; // maximum name length
		GLsizei length; // name length

		glGetProgramiv(shader->id, GL_ACTIVE_UNIFORMS, &count);
		glGetProgramiv(shader->id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);

		auto* name = new GLchar[bufSize];

		for (int i = 0; i < count; i++) {
			glGetActiveUniform(shader->id, static_cast<GLuint>(i), bufSize, &length, &size, &type, name);
			shader->uniformLocations[name] = glGetUniformLocation(shader->id, name);
		}
		
		delete[] name;

		return shader;
	}

	std::unique_ptr<gl::IUniformBlock> ShaderCreator::createUniformBlock(const std::string& name)
	{
		auto uniformBuffer = std::unique_ptr<UniformBuffer>(new UniformBuffer(lockContextPtr()));

		glGenBuffers(1, &uniformBuffer->id);
		uniformBuffer->name = name;
		uniformBuffer->ubo = uniformBuffer->id;

		lockContextPtr()->linkUbo(uniformBuffer->id);

		return uniformBuffer;
	}
}
#include <iostream>
#include "ShaderCreator.h"

#include "Context.h"
#include "Shader.h"

namespace opengl
{
#if 0
	gl::Shader ShaderCreator::loadFromFile(std::string vertex_path, std::string fragment_path, std::string geometry_path)
	{
		std::string vertexCode(""), fragmentCode(""), geometryCode("");
		std::string vertexCodeLine, fragmentCodeLine, geometryCodeLine;


		sys::File vertexFileHandle = systemHandle->open(vertex_path);
		if (!vertexFileHandle.is_open()) {
			throw sys::FileException(sys::FileError::FILE_NOT_FOUND, "", vertex_path);
		}

		std::stringstream vertexFile;
		vertexFileHandle.read(vertexFile);
		while (getline(vertexFile, vertexCodeLine))
		{
			vertexCode += vertexCodeLine + "\n";
		}

		vertexFileHandle.close();


		sys::File fragmentFileHandle = systemHandle->open(fragment_path);
		if (!fragmentFileHandle.is_open()) {
			throw sys::FileException(sys::FileError::FILE_NOT_FOUND, "", fragment_path);
		}

		std::stringstream fragmentFile;
		fragmentFileHandle.read(fragmentFile);
		while (getline(fragmentFile, fragmentCodeLine))
		{
			fragmentCode += fragmentCodeLine + "\n";
		}

		fragmentFileHandle.close();


		if (geometry_path != "") {
			sys::File geometryFileHandle = systemHandle->open(geometry_path);
			if (!geometryFileHandle.is_open()) {
				throw sys::FileException(sys::FileError::FILE_NOT_FOUND, "", geometry_path);
			}

			std::stringstream geometryFile;
			geometryFileHandle.read(geometryFile);
			while (getline(geometryFile, geometryCodeLine))
			{
				geometryCode += geometryCodeLine + "\n";
			}

			geometryFileHandle.close();
		}

		return context->createShader(vertexCode, fragmentCode, geometryCode);
	}
#endif
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
}
#include "GL3_ShaderLoader.h"

namespace gl3
{
	GL3_ShaderLoader::GL3_ShaderLoader(sys::PlatformHandle* systemHandle, GL3_Context* glContext)
		: systemHandle(systemHandle), glContext(glContext) {}

	GL3_Shader GL3_ShaderLoader::loadFromFile(std::string vertex_path, std::string fragment_path, std::string geometry_path)
	{
		std::string vertexCode(""), fragmentCode(""), geometryCode("");
		std::string vertexCodeLine, fragmentCodeLine, geometryCodeLine;


		sys::File vertexFileHandle = systemHandle->open(vertex_path);
		if (!vertexFileHandle.is_open()) {
			throw FileException(FileError::PATH_VIOLATION, vertex_path);
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
			throw FileException(FileError::PATH_VIOLATION, fragment_path);
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
				throw FileException(FileError::PATH_VIOLATION, geometry_path);
			}

			std::stringstream geometryFile;
			geometryFileHandle.read(geometryFile);
			while (getline(geometryFile, geometryCodeLine))
			{
				geometryCode += geometryCodeLine + "\n";
			}

			geometryFileHandle.close();
		}

		return glContext->compileShader(vertexCode, fragmentCode, geometryCode);
	}

	GL3_Shader GL3_ShaderLoader::loadFromString(std::string vertex_string, std::string fragment_string, std::string geometry_string)
	{
		return glContext->compileShader(vertex_string, fragment_string, geometry_string);
	}
}
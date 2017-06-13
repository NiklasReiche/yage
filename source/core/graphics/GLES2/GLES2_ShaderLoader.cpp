#include "GLES2_ShaderLoader.h"

namespace gles2
{
	GLES2_ShaderLoader::GLES2_ShaderLoader(platform::PlatformHandle* systemHandle, GLES2_Context* glContext)
		: systemHandle(systemHandle), glContext(glContext) {}

	GLES2_Shader GLES2_ShaderLoader::loadShader(std::string vertex_path, std::string fragment_path)
	{
		std::vector<std::string> attributes;
		std::string vertexCode(""), fragmentCode("");
		std::string vertexCodeLine, fragmentCodeLine;
		// File Handle
		try {
			std::stringstream vertexFile, fragmentFile;
			systemHandle->read(vertex_path, vertexFile);
			systemHandle->read(vertex_path, fragmentFile);

			while (getline(vertexFile, vertexCodeLine))
			{
				vertexCode += vertexCodeLine + "\n";

				std::stringstream line(vertexCodeLine);
				std::string qualifier;
				line >> qualifier;
				if (qualifier == "attribute") {
					std::string type, name;
					line >> type;
					line >> name;
					attributes.push_back(name);
				}
			}

			while (getline(fragmentFile, fragmentCodeLine))
			{
				fragmentCode += fragmentCodeLine + "\n";
			}
		}
		catch (std::exception err) {
			systemHandle->log("ERROR::SHADER_LOADER: File Handling Error");
		}

		return glContext->compileShader(vertexCode, fragmentCode, attributes);
	}
}
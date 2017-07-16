#include "GL3_ShaderLoader.h"

namespace gl3
{
	GL3_ShaderLoader::GL3_ShaderLoader(GL3_Context* glContext)
		: glContext(glContext) {}

	GL3_Shader GL3_ShaderLoader::loadShader(std::string vertex_path, std::string fragment_path, std::string geometry_path)
	{
		std::string vertexCode(""), fragmentCode(""), geometryCode("");
		std::string vertexCodeLine, fragmentCodeLine, geometryCodeLine;
		// File Handle
		try {
			std::ifstream vertexFile, fragmentFile, geometryFile;

			vertexFile.open(vertex_path);
			while (getline(vertexFile, vertexCodeLine))
			{
				vertexCode += vertexCodeLine + "\n";
			}
			vertexFile.close();

			fragmentFile.open(fragment_path);
			while (getline(fragmentFile, fragmentCodeLine))
			{
				fragmentCode += fragmentCodeLine + "\n";
			}
			fragmentFile.close();

			if (geometry_path != "") {
				geometryFile.open(geometry_path);
				while (getline(geometryFile, geometryCodeLine))
				{
					geometryCode += geometryCodeLine + "\n";
				}
				geometryFile.close();
			}
		}
		catch (std::exception err) {
			std::cout << "ERROR::SHADER_LOADER: File Handling Error" << std::endl;
		}

		return glContext->compileShader(vertexCode, fragmentCode, geometryCode);
	}
}
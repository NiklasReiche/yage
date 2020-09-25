#pragma once

#include <string>

#include "OpenGL.h"

#include "../ShaderCreator.h"
#include "GL3_BaseObject.h"
#include "GL3_Exception.h"

namespace gl3
{
	class GL3_ShaderCreator final : public GL3_BaseObject, public gl::IShaderCreator
	{
	public:
		std::unique_ptr<gl::IShader> createShader(
			const std::string& vertexCode, 
			const std::string& fragmentCode, 
			const std::string& geometryCode = "") override;

	private:
		using GL3_BaseObject::GL3_BaseObject;
		
		typedef ShaderCompilationException::ShaderType ShaderType;
		static void checkShaderCompilationError(GLuint program, ShaderType type);

		friend class GL3_Context;
	};
}

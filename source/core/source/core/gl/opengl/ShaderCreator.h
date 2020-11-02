#pragma once

#include <string>

#include "OpenGL.h"

#include "../ShaderCreator.h"
#include "BaseObject.h"
#include "GlException.h"

namespace opengl
{
	class ShaderCreator final : public BaseObject, public gl::IShaderCreator
	{
	public:
		std::unique_ptr<gl::IShader> createShader(
			const std::string& vertexCode, 
			const std::string& fragmentCode, 
			const std::string& geometryCode = "") override;

	private:
		using BaseObject::BaseObject;
		
		typedef ShaderCompilationException::ShaderType ShaderType;
		static void checkShaderCompilationError(GLuint program, ShaderType type);

		friend class Context;
	};
}

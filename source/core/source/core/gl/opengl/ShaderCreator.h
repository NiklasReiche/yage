#pragma once

#include <string>

#include "../ShaderCreator.h"
#include "OpenGL.h"
#include "BaseObject.h"
#include "GlException.h"

namespace yage::opengl
{
	class ShaderCreator final : public BaseObject, public gl::IShaderCreator
	{
	public:
		std::unique_ptr<gl::IShader> createShader(const std::string& vertexCode, const std::string& fragmentCode,
			const std::string& geometryCode) override;

		std::unique_ptr<gl::IUniformBlock> createUniformBlock(const std::string& name) override;

	private:
		using BaseObject::BaseObject;
		
		typedef ShaderCompilationException::ShaderType ShaderType;
		static void checkShaderCompilationError(GLuint program, ShaderType type);

		friend class Context;
	};
}

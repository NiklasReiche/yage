#pragma once

#include <memory>
#include <string>

#include "Shader.h"

namespace gl
{
	class IShaderCreator
	{
	public:
		virtual ~IShaderCreator() = default;
		IShaderCreator(IShaderCreator&& other) = default;
		IShaderCreator& operator=(IShaderCreator&& other) = default;

		[[nodiscard]]
		virtual std::unique_ptr<IShader> createShader(
			const std::string& vertexCode, 
			const std::string& fragmentCode,
			const std::string& geometryCode = "") = 0;

	protected:
		IShaderCreator() = default;
		IShaderCreator(const IShaderCreator& other) = default;
		IShaderCreator& operator=(const IShaderCreator& other) = default;
	};
}

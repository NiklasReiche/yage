#pragma once

#include <memory>
#include <string>

#include "Shader.h"
#include "IUniformBlock.h"

namespace yage::gl
{
	class IShaderCreator
	{
	public:
		virtual ~IShaderCreator() = default;
		IShaderCreator(IShaderCreator&& other) = default;
		IShaderCreator& operator=(IShaderCreator&& other) = default;

		[[nodiscard]]
		virtual std::unique_ptr<IShader> createShader(const std::string& vertexCode, const std::string& fragmentCode,
			const std::string& geometryCode = "") = 0;

		[[nodiscard]]
		virtual std::unique_ptr<IUniformBlock> createUniformBlock(const std::string& name) = 0;

	protected:
		IShaderCreator() = default;
		IShaderCreator(const IShaderCreator& other) = default;
		IShaderCreator& operator=(const IShaderCreator& other) = default;
	};
}

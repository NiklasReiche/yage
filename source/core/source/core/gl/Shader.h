#pragma once

#include <string>

#include <gml/vector.h>
#include <gml/matrix.h>

#include "IUniformBlock.h"

namespace gl
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		/**
		 * @brief Checks whether the shader contains the given uniform.
		 *
		 * @param name the uniform's name
		 * @return true when the shader contains the uniform
		 * @return false when the shader does not contain the uniform
		 */
		[[nodiscard]]
		virtual bool hasUniform(const std::string& name) const = 0;

		virtual void setUniform(const std::string& name, int value) = 0;
		virtual void setUniform(const std::string& name, bool value) = 0;
		virtual void setUniform(const std::string& name, float value) = 0;
		virtual void setUniform(const std::string& name, gml::Vec3f value) = 0;
		virtual void setUniform(const std::string& name, gml::Mat4f value) = 0;

		virtual void linkUniformBlock(const IUniformBlock& uniformBlock) = 0;

	protected:
		IShader() = default;
		IShader(const IShader& other) = default;
		IShader(IShader&& other) = default;
		IShader& operator=(const IShader& other) = default;
		IShader& operator= (IShader&& other) = default;
	};
}

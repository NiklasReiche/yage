#pragma once

#include <stdexcept>
#include <string>

namespace opengl
{
	enum GlErrorCodes {
		GLFW_ERROR,
		GLAD_ERROR
	};

	class GlException final : public std::runtime_error
	{
	public:
		explicit GlException(const int code, const std::string& msg = "")
			: std::runtime_error(msg), errorCode(code) {}

		[[nodiscard]]
		int getErrorCode() const
		{
			return errorCode;
		}

	private:
		int errorCode;
	};

	class ShaderCompilationException final : public std::runtime_error
	{
	public:
		enum class ShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			GEOMETRY_SHADER,
			SHADER_PROGRAM
		};
		
		explicit ShaderCompilationException(const ShaderType type, const std::string& info)
			: std::runtime_error(info), type(type) {}

		[[nodiscard]]
		ShaderType getType() const
		{
			return type;
		}
		
	private:
		ShaderType type;
	};
}

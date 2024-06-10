#pragma once

#include <string>
#include <map>

#include "../Shader.h"
#include "OpenGlObject.h"
#include "OpenGL.h"

namespace yage::opengl
{
	class Shader : public OpenGlObject, public gl::IShader, public std::enable_shared_from_this<Shader>
	{
	public:
		virtual ~Shader();
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;

		bool hasUniform(const std::string& name) const override;

		void setUniform(const std::string& name, int value) override;
		void setUniform(const std::string& name, bool value) override;
		void setUniform(const std::string& name, float value) override;
		void setUniform(const std::string& name, math::Vec3f value) override;
        void setUniform(const std::string& name, math::Vec4f value) override;
		void setUniform(const std::string& name, math::Mat4f value) override;

		void linkUniformBlock(const gl::IUniformBlock& uniformBlock) override;

	private:
		GLuint& program = OpenGlObject::id;
		std::map<std::string, GLint> uniformLocations;

		using OpenGlObject::OpenGlObject;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		friend class Context;
		friend class Renderer;
		friend class ShaderCreator;
	};
}

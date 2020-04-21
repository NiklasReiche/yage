#pragma once

#include <string>
#include <map>

#include <platform/OpenGL.h>
#include "GL3_Object.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "../Shader.h"

namespace gl3
{
	class GL3_Shader : public GL3_Object, public gl::IShader, public std::enable_shared_from_this<GL3_Shader>
	{
	public:
		virtual ~GL3_Shader();
		GL3_Shader(const GL3_Shader& other) = delete;
		GL3_Shader& operator=(const GL3_Shader& other) = delete;

		bool hasUniform(const std::string& name) const override;

		void setUniform(const std::string& name, int value) override;
		void setUniform(const std::string& name, bool value) override;
		void setUniform(const std::string& name, float value) override;
		void setUniform(const std::string& name, gml::Vec3f value) override;
		void setUniform(const std::string& name, gml::Mat4f value) override;

	private:
		GLuint& program = GL3_Object::id;
		std::map<std::string, GLint> uniformLocations;

		using GL3_Object::GL3_Object;

		GL3_Shader(GL3_Shader&& other) noexcept;
		GL3_Shader& operator=(GL3_Shader&& other) noexcept;

		friend class GL3_Context;
		friend class GL3_Renderer;
		friend class GL3_ShaderCreator;
	};
}
